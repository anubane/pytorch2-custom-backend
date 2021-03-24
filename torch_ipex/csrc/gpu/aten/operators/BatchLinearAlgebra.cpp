#include <ATen/Context.h>
#include <ATen/NativeFunctions.h>
#include <ATen/native/LinearAlgebraUtils.h>

#include <core/ApplyUtils.h>
#include <core/Context.h>
#include <utils/ATDispatch.h>
#include <utils/Numerics.h>

#ifdef USE_ONEMKL
#include <mkl.h>
#include <oneapi/mkl.hpp>
#endif

using namespace at::dpcpp;

DPCPP_DEF_K2(triuTrilSycl, typename scalar_t, typename IndexType, bool upper);

namespace at {
namespace AtenIpexTypeXPU {
namespace impl {

template <typename scalar_t, typename IndexType, bool upper>
void triu_tril_dpcpp_kernel(
    scalar_t* result,
    const scalar_t* src,
    const IndexType stride0,
    const IndexType stride1,
    const int64_t k,
    const int64_t N) {
  auto queue = dpcppGetCurrentQueue();
  int64_t group_size = dpcppMaxWorkGroupSize(queue);
  auto num_groups = CeilDiv(N, group_size);
  auto total_items = num_groups * group_size;

  auto cgf = DPCPP_Q_CGF(cgh) {
    auto result_data = get_buffer<dpcpp_w_mode>(cgh, result);
    auto src_data = get_buffer<dpcpp_r_mode>(cgh, src);

    auto kfn = DPCPP_Q_KFN(DPCPP::nd_item<1> item) {
      auto result_ptr = get_pointer(result_data);
      auto src_ptr = get_pointer(src_data);

      for (size_t linearIndex = item.get_global_id(0); linearIndex < (size_t)N;
           linearIndex += item.get_global_range()[0]) {
        IndexType row, col;

        if (stride0 > stride1) {
          row = (IndexType)(linearIndex / stride0);
          col = (IndexType)(linearIndex % stride0 / stride1);
        } else {
          row = (IndexType)((linearIndex % stride1) / stride0);
          col = (IndexType)(linearIndex / stride1);
        }

        bool mask = upper ? (col - row >= k) : (col - row <= k);
        result_ptr[linearIndex] = mask ? src_ptr[linearIndex] : scalar_t(0);
      }
    };

    // kick off kernel
    cgh.parallel_for<DPCPP_K(triuTrilSycl, scalar_t, IndexType, upper)>(
        DPCPP::nd_range<1>(
            DPCPP::range<1>(total_items), DPCPP::range<1>(group_size)),
        kfn);
  };

  DPCPP_Q_ASYNC_SUBMIT(queue, cgf);
}

template <bool upper>
Tensor& triu_tril_dpcpp_template(
    Tensor& result,
    const Tensor& self,
    int64_t k,
    const char* name) {
  int64_t N = self.numel();

  IPEX_DISPATCH_ALL_TYPES_AND2(
      at::ScalarType::Half,
      at::ScalarType::Bool,
      self.scalar_type(),
      name,
      [&] {
        if (dpcpp::detail::canUse32BitIndexMath(self)) {
          auto self_info =
              dpcpp::detail::getTensorInfo<scalar_t, int32_t>(self);
          triu_tril_dpcpp_kernel<scalar_t, int32_t, upper>(
              result.data_ptr<scalar_t>(),
              self.data_ptr<scalar_t>(),
              self_info.strides[0],
              self_info.strides[1],
              k,
              N);
        } else {
          auto self_info =
              dpcpp::detail::getTensorInfo<scalar_t, int64_t>(self);
          triu_tril_dpcpp_kernel<scalar_t, int64_t, upper>(
              result.data_ptr<scalar_t>(),
              self.data_ptr<scalar_t>(),
              self_info.strides[0],
              self_info.strides[1],
              k,
              N);
        }
      });

  return result;
}

Tensor& tril_dpcpp_out(Tensor& result, const Tensor& self, int64_t k) {
  if (result.sizes() != self.sizes()) {
    result.resize_as_(self);
  }
  if (self.numel() == 0) {
    return result;
  }

  return triu_tril_dpcpp_template<false>(result, self, k, "tril");
}

Tensor& tril_dpcpp_(Tensor& self, int64_t k) {
  return tril_dpcpp_out(self, self, k);
}

Tensor& triu_dpcpp_out(Tensor& result, const Tensor& self, int64_t k) {
  if (result.sizes() != self.sizes()) {
    result.resize_as_(self);
  }
  if (self.numel() == 0) {
    return result;
  }
  return triu_tril_dpcpp_template<true>(result, self, k, "triu");
}

Tensor& triu_dpcpp_(Tensor& self, int64_t k) {
  return triu_dpcpp_out(self, self, k);
}

template <typename scalar_t>
static void apply_lu_dpcpp_(Tensor& self_, Tensor& pivots_, Tensor& infos_) {
#ifdef USE_ONEMKL
  auto& dpcpp_queue = getCurrentDPCPPStream().dpcpp_queue();
  std::int64_t batch_size = native::batchCount(self_);
  std::int64_t m = self_.size(-2);
  std::int64_t n = self_.size(-1);
  scalar_t* a = (scalar_t*)(self_.data_ptr());
  std::int64_t lda = m;
  std::int64_t stride_a = lda * n;
  std::int64_t* ipiv = (std::int64_t*)(pivots_.data_ptr());
  std::int64_t stride_ipiv = (m < n) ? m : n;
  std::int64_t scratchpadsize =
      oneapi::mkl::lapack::getrf_batch_scratchpad_size<scalar_t>(
          dpcpp_queue, m, n, lda, stride_a, stride_ipiv, batch_size);
  Tensor scratchpad_at = at::empty({scratchpadsize}, self_.options());
  DPCPP_ONEMKL_SUBMIT(
      dpcpp_queue,
      oneapi::mkl::lapack::getrf_batch,
      dpcpp_queue,
      m,
      n,
      a,
      lda,
      stride_a,
      ipiv,
      stride_ipiv,
      batch_size,
      (scalar_t*)(scratchpad_at.data_ptr()),
      scratchpadsize);
#else
  AT_ERROR("lu: oneMKL library not found in compilation");
#endif
}

template <typename scalar_t>
static void apply_svd(
    Tensor& self,
    Tensor& U,
    Tensor& S,
    Tensor& VT,
    char jobz,
    std::vector<int64_t>& infos) {
#ifdef USE_ONEMKL
  auto& dpcpp_queue = getCurrentDPCPPStream().dpcpp_queue();
  using value_t = typename c10::scalar_value_type<scalar_t>::type;
  scalar_t* self_data = (scalar_t*)(self.data_ptr());
  auto U_data = U.data_ptr<scalar_t>();
  auto S_data = S.data_ptr<value_t>();
  auto VT_data = VT.data_ptr<scalar_t>();
  auto m = self.size(-2);
  auto n = self.size(-1);
  auto mn = std::min(m, n);
  std::int64_t lda = m;
  std::int64_t ldu = n;
  std::int64_t ldvt = mn;
  oneapi::mkl::jobsvd jobu, jobvt;
  if (jobz == 'N') {
    jobu = oneapi::mkl::jobsvd::N;
    jobvt = oneapi::mkl::jobsvd::N;
  } else if (jobz == 'S') {
    jobu = oneapi::mkl::jobsvd::S;
    jobvt = oneapi::mkl::jobsvd::S;
  } else {
    jobu = oneapi::mkl::jobsvd::A;
    jobvt = oneapi::mkl::jobsvd::A;
  }
  std::int64_t scratchpadsize =
      oneapi::mkl::lapack::gesvd_scratchpad_size<scalar_t>(
          dpcpp_queue, jobu, jobvt, m, n, lda, ldu, ldvt);
  Tensor scratchpad_at = at::empty({scratchpadsize}, self.options());
  DPCPP_ONEMKL_SUBMIT(
      dpcpp_queue,
      oneapi::mkl::lapack::gesvd,
      dpcpp_queue,
      jobu,
      jobvt,
      m,
      n,
      (scalar_t*)(self.data_ptr()),
      lda,
      S_data,
      U_data,
      ldu,
      VT_data,
      ldvt,
      (scalar_t*)(scratchpad_at.data_ptr()),
      scratchpadsize);
#else
  AT_ERROR("svd: oneMKL library not found in compilation");
#endif
}
} // namespace impl

Tensor& triu_out(Tensor& out, const Tensor& self, int64_t diagonal) {
  impl::triu_dpcpp_out(out, self, diagonal);
  return out;
}

Tensor& tril_out(Tensor& out, const Tensor& self, int64_t diagonal) {
  impl::tril_dpcpp_out(out, self, diagonal);
  return out;
}

Tensor& tril_(Tensor& self, int64_t diagonal) {
  return at::AtenIpexTypeXPU::tril_out(self, self, diagonal);
}

Tensor& triu_(Tensor& self, int64_t diagonal) {
  return at::AtenIpexTypeXPU::triu_out(self, self, diagonal);
}

std::tuple<Tensor, Tensor, Tensor> _lu_with_info(
    const Tensor& self,
    bool pivot,
    bool check_errors) {
  TORCH_CHECK(pivot, "lu without pivoting is not implemented on the DPCPP");
  TORCH_CHECK(
      self.dim() >= 2,
      "expected tensor with 2 or more dimensions, got size: ",
      self.sizes(),
      " instead");
  native::squareCheckInputs(self);
  auto req_size = self.sizes().vec();
  req_size.pop_back();
  auto pivots_tensor = at::empty(req_size, self.options().dtype(kLong));
  req_size.pop_back();
  auto infos_tensor = at::zeros(req_size, self.options().dtype(kLong));

  Tensor self_working_copy;
  if (self.numel() == 0) {
    self_working_copy = at::empty_like(self);
  } else {
    self_working_copy = native::cloneBatchedColumnMajor(self);
    IPEX_DISPATCH_FLOATING_TYPES(self.scalar_type(), "lu_dpcpp", [&] {
      impl::apply_lu_dpcpp_<scalar_t>(
          self_working_copy, pivots_tensor, infos_tensor);
    });
  }
  if (check_errors) {
    if (self.dim() > 2) {
      native::batchCheckErrors(infos_tensor, "lu");
    } else {
      native::singleCheckErrors(infos_tensor.item<int64_t>(), "lu");
    }
  }
  return std::make_tuple(self_working_copy, pivots_tensor, infos_tensor);
}

std::tuple<Tensor, Tensor, Tensor> _svd_helper(
    const Tensor& self,
    bool some,
    bool compute_uv) {
  std::vector<int64_t> infos(native::batchCount(self), 0);
  int64_t m = self.size(-2), n = self.size(-1);
  int64_t k = std::min(m, n);

  char jobz = compute_uv ? (some ? 'S' : 'A') : 'N';

  Tensor U_working_copy, S_working_copy, VT_working_copy;
  std::tie(U_working_copy, S_working_copy, VT_working_copy) =
      native::_create_U_S_VT(self, some, compute_uv);

  if (self.numel() > 0) {
    auto self_working_copy = native::cloneBatchedColumnMajor(self);

    AT_DISPATCH_FLOATING_TYPES(self.scalar_type(), "svd_xpu", [&] {
      impl::apply_svd<scalar_t>(
          self_working_copy,
          U_working_copy,
          S_working_copy,
          VT_working_copy,
          jobz,
          infos);
    });

    if (self.dim() > 2) {
      native::batchCheckErrors(infos, "svd_xpu");
    } else {
      native::singleCheckErrors(infos[0], "svd_xpu");
    }

    if (compute_uv) {
      if (some) {
        VT_working_copy = VT_working_copy.narrow(-1, 0, k);
      }
    } else {
      VT_working_copy.zero_();
      U_working_copy.zero_();
    }
  } else {
    U_working_copy.zero_();
    VT_working_copy.zero_();
  }
  return std::make_tuple(U_working_copy, S_working_copy, VT_working_copy);
}

std::tuple<Tensor, Tensor, Tensor> svd(
    const Tensor& self,
    bool some,
    bool compute_uv) {
  TORCH_CHECK(
      self.dim() >= 2,
      "self should have at least 2 dimensions, but has ",
      self.dim(),
      " dimensions instead");
  return at::_svd_helper(self, some, compute_uv);
}

std::tuple<Tensor&, Tensor&, Tensor&> svd_out(
    Tensor& U,
    Tensor& S,
    Tensor& VT,
    const Tensor& self,
    bool some,
    bool compute_uv) {
  TORCH_CHECK(
      self.dim() >= 2,
      "self should have at least 2 dimensions, but has ",
      self.dim(),
      " dimensions instead");
  Tensor U_tmp, S_tmp, VT_tmp;
  std::tie(U_tmp, S_tmp, VT_tmp) = at::_svd_helper(self, some, compute_uv);
  U.resize_as_(U_tmp).copy_(U_tmp);
  S.resize_as_(S_tmp).copy_(S_tmp);
  VT.resize_as_(VT_tmp).copy_(VT_tmp);
  return std::tuple<Tensor&, Tensor&, Tensor&>(U, S, VT);
}

} // namespace AtenIpexTypeXPU
} // namespace at

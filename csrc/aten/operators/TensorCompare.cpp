#include <ATen/ATen.h>
#include "comm/AccumulateType.h"

#include <core/ApplyUtils.h>
#include <core/Context.h>
#include <core/DPCPPUtils.h>
#include <core/Memory.h>

#include "comm/ATDispatch.h"


using namespace xpu::dpcpp;

namespace at {
namespace AtenIpexTypeXPU {
namespace impl {

template <typename scalar_t, typename scalar1_t>
class where_functor {
 public:
  where_functor() {}
  void operator()(
      scalar_t& ret_val,
      const scalar1_t& cond_val,
      const scalar_t& self_val,
      const scalar_t& other_val) const {
    ret_val = cond_val ? self_val : other_val;
  }
};

template <typename scalar_t>
void _s_where(
    at::Tensor& ret,
    const at::Tensor& condition,
    const at::Tensor& self,
    const at::Tensor& other) {
  if (condition.scalar_type() == at::ScalarType::Byte) {
    DPCPP_tensor_apply4<scalar_t, uint8_t, scalar_t, scalar_t>(
        ret, condition, self, other, where_functor<scalar_t, uint8_t>());
  } else {
    DPCPP_tensor_apply4<scalar_t, bool, scalar_t, scalar_t>(
        ret, condition, self, other, where_functor<scalar_t, bool>());
  }
}

} // namespace impl

Tensor _s_where(
    const Tensor& condition,
    const Tensor& self,
    const Tensor& other) {
  Tensor ret = at::empty(self.sizes(), self.options());
  IPEX_DISPATCH_ALL_TYPES_AND3(
      at::ScalarType::Half,
      at::ScalarType::BFloat16,
      at::ScalarType::Bool,
      ret.scalar_type(),
      "where",
      [&] { impl::_s_where<scalar_t>(ret, condition, self, other); });
  return ret;
}

Tensor isnan(const Tensor& self) {
  return self != self;
}

} // namespace AtenIpexTypeXPU
} // namespace at

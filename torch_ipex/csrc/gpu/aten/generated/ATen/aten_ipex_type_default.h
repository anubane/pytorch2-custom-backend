// Autogenerated file by gen-gpu-ops.py. Do not edit directly!

#include <ATen/Tensor.h>

namespace at {

class AtenIpexTypeDefault {
 public:
  static at::Tensor add(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor & add_(at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor & add_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor add(const at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor & add_(at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor & arange_out(at::Tensor & out, at::Scalar start, at::Scalar end, at::Scalar step);
  static at::Tensor as_strided(const at::Tensor & self, at::IntArrayRef size, at::IntArrayRef stride, c10::optional<int64_t> storage_offset);
  static at::Tensor bitwise_not(const at::Tensor & self);
  static at::Tensor & bitwise_not_(at::Tensor & self);
  static at::Tensor & bitwise_not_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor logical_not(const at::Tensor & self);
  static at::Tensor & logical_not_(at::Tensor & self);
  static at::Tensor & logical_not_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor convolution_overrideable(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & bias, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool transposed, at::IntArrayRef output_padding, int64_t groups);
  static at::Tensor & copy_(at::Tensor & self, const at::Tensor & src, bool non_blocking);
  static at::Tensor div(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & div_(at::Tensor & self, const at::Tensor & other);
  static at::Tensor & div_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor empty(at::IntArrayRef size, const at::TensorOptions & options, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor & resize_(at::Tensor & self, at::IntArrayRef size, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor empty_strided(at::IntArrayRef size, at::IntArrayRef stride, const at::TensorOptions & options);
  static at::Tensor & eye_out(at::Tensor & out, int64_t n);
  static at::Tensor & eye_out(at::Tensor & out, int64_t n, int64_t m);
  static at::Tensor & fill_(at::Tensor & self, at::Scalar value);
  static at::Tensor & fill_(at::Tensor & self, const at::Tensor & value);
  static at::Tensor & linspace_out(at::Tensor & out, at::Scalar start, at::Scalar end, int64_t steps);
  static at::Tensor & logspace_out(at::Tensor & out, at::Scalar start, at::Scalar end, int64_t steps, double base);
  static at::Tensor _log_softmax(const at::Tensor & self, int64_t dim, bool half_to_float);
  static at::Tensor mul(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & mul_(at::Tensor & self, const at::Tensor & other);
  static at::Tensor & mul_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor mul(const at::Tensor & self, at::Scalar other);
  static at::Tensor & mul_(at::Tensor & self, at::Scalar other);
  static std::tuple<at::Tensor,at::Tensor,at::Tensor> native_batch_norm(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & bias, const at::Tensor & running_mean, const at::Tensor & running_var, bool training, double momentum, double eps);
  static at::Tensor & range_out(at::Tensor & out, at::Scalar start, at::Scalar end, at::Scalar step);
  static at::Tensor neg(const at::Tensor & self);
  static at::Tensor & neg_(at::Tensor & self);
  static at::Tensor & neg_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor & relu_(at::Tensor & self);
  static at::Tensor sigmoid(const at::Tensor & self);
  static at::Tensor & sigmoid_(at::Tensor & self);
  static at::Tensor & sigmoid_out(at::Tensor & out, const at::Tensor & self);
  static at::Tensor sum(const at::Tensor & self, c10::optional<at::ScalarType> dtype);
  static at::Tensor sum(const at::Tensor & self, at::IntArrayRef dim, bool keepdim, c10::optional<at::ScalarType> dtype);
  static at::Tensor & sum_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef dim, bool keepdim, c10::optional<at::ScalarType> dtype);
  static at::Tensor threshold(const at::Tensor & self, at::Scalar threshold, at::Scalar value);
  static at::Tensor & threshold_(at::Tensor & self, at::Scalar threshold, at::Scalar value);
  static at::Tensor & threshold_out(at::Tensor & out, const at::Tensor & self, at::Scalar threshold, at::Scalar value);
  static at::Tensor roll(const at::Tensor & self, at::IntArrayRef shifts, at::IntArrayRef dims);
  static at::Tensor norm(const at::Tensor & self, c10::optional<at::Scalar> p, at::ScalarType dtype);
  static at::Tensor norm(const at::Tensor & self, at::Scalar p);
  static at::Tensor norm(const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim, at::ScalarType dtype);
  static at::Tensor norm(const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim);
  static at::Tensor & norm_out(at::Tensor & out, const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim, at::ScalarType dtype);
  static at::Tensor & norm_out(at::Tensor & out, const at::Tensor & self, c10::optional<at::Scalar> p, at::IntArrayRef dim, bool keepdim);
  static at::Tensor & resize_as_(at::Tensor & self, const at::Tensor & the_template, c10::optional<at::MemoryFormat> memory_format);
  static at::Tensor & zero_(at::Tensor & self);
  static at::Tensor & sub_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor sub(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor & sub_(at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor sub(const at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor & sub_(at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor rsub(const at::Tensor & self, const at::Tensor & other, at::Scalar alpha);
  static at::Tensor rsub(const at::Tensor & self, at::Scalar other, at::Scalar alpha);
  static at::Tensor addmm(const at::Tensor & self, const at::Tensor & mat1, const at::Tensor & mat2, at::Scalar beta, at::Scalar alpha);
  static at::Scalar _local_scalar_dense(const at::Tensor & self);
  static at::Tensor view(const at::Tensor & self, at::IntArrayRef size);
  static at::Tensor & tril_(at::Tensor & self, int64_t diagonal);
  static at::Tensor & triu_(at::Tensor & self, int64_t diagonal);
  static at::Tensor & addcdiv_(at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & triu_out(at::Tensor & out, const at::Tensor & self, int64_t diagonal);
  static at::Tensor & tril_out(at::Tensor & out, const at::Tensor & self, int64_t diagonal);
  static at::Tensor tril_indices(int64_t row, int64_t col, int64_t offset, const at::TensorOptions & options);
  static at::Tensor triu_indices(int64_t row, int64_t col, int64_t offset, const at::TensorOptions & options);
  static at::Tensor & ne_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor ne(const at::Tensor & self, at::Scalar other);
  static at::Tensor & ne_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor ne(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & eq_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor eq(const at::Tensor & self, at::Scalar other);
  static at::Tensor & eq_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor eq(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & ge_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor ge(const at::Tensor & self, at::Scalar other);
  static at::Tensor & ge_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor ge(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & le_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor le(const at::Tensor & self, at::Scalar other);
  static at::Tensor & le_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor le(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & gt_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor gt(const at::Tensor & self, at::Scalar other);
  static at::Tensor & gt_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor gt(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & lt_out(at::Tensor & out, const at::Tensor & self, at::Scalar other);
  static at::Tensor lt(const at::Tensor & self, at::Scalar other);
  static at::Tensor & lt_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & other);
  static at::Tensor lt(const at::Tensor & self, const at::Tensor & other);
  static at::Tensor & addcmul_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor addcmul(const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & addcmul_(at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor & addcdiv_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static at::Tensor addcdiv(const at::Tensor & self, const at::Tensor & tensor1, const at::Tensor & tensor2, at::Scalar value);
  static std::tuple<at::Tensor &,at::Tensor &> topk_out(at::Tensor & values, at::Tensor & indices, const at::Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted);
  static std::tuple<at::Tensor,at::Tensor> topk(const at::Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted);
  static std::tuple<at::Tensor,at::Tensor> nll_loss_forward(const at::Tensor & self, const at::Tensor & target, const at::Tensor & weight, int64_t reduction, int64_t ignore_index);
  static at::Tensor & adaptive_avg_pool2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor adaptive_avg_pool2d(const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor _adaptive_avg_pool2d(const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor & avg_pool2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override);
  static at::Tensor avg_pool2d(const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, bool ceil_mode, bool count_include_pad, c10::optional<int64_t> divisor_override);
  static std::tuple<at::Tensor &,at::Tensor &> max_pool2d_with_indices_out(at::Tensor & out, at::Tensor & indices, const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode);
  static std::tuple<at::Tensor,at::Tensor> max_pool2d_with_indices(const at::Tensor & self, at::IntArrayRef kernel_size, at::IntArrayRef stride, at::IntArrayRef padding, at::IntArrayRef dilation, bool ceil_mode);
  static at::Tensor & upsample_nearest2d_out(at::Tensor & out, const at::Tensor & self, at::IntArrayRef output_size);
  static at::Tensor upsample_nearest2d(const at::Tensor & self, at::IntArrayRef output_size);

};

void RegisterAtenTypeFunctions();

}  // namespace at


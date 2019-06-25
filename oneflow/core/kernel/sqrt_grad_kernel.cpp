#include "oneflow/core/kernel/sqrt_grad_kernel.h"

namespace oneflow {

template<DeviceType device_type, typename T>
void SqrtGradKernel<device_type, T>::ForwardDataContent(
    const KernelCtx& ctx, std::function<Blob*(const std::string&)> BnInOp2Blob) const {
  Blob* out_blob = BnInOp2Blob("out");
  Blob* out_diff_blob = BnInOp2Blob("out_diff");
  Blob* in_diff_blob = BnInOp2Blob("in_diff");
  KernelUtil<device_type, T>::Div(ctx.device_ctx, out_blob->static_shape().elem_cnt(),
                                  out_diff_blob->dptr<T>(), out_blob->dptr<T>(),
                                  in_diff_blob->mut_dptr<T>());
  KernelUtil<device_type, T>::Scal(ctx.device_ctx, out_blob->static_shape().elem_cnt(),
                                   static_cast<T>(0.5), in_diff_blob->mut_dptr<T>(), 1);
}

ADD_DEFAULT_KERNEL_CREATOR(OperatorConf::kSqrtGradConf, SqrtGradKernel, FLOATING_DATA_TYPE_SEQ);

}  // namespace oneflow

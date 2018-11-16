#ifndef ONEFLOW_CORE_KERNEL_BROADCAST_ADD_XPU_UTIL
#define ONEFLOW_CORE_KERNEL_BROADCAST_ADD_XPU_UTIL

#include "oneflow/core/ndarray/xpu_ndarray_builder.h"

namespace oneflow {

template<typename T, int NDIMS>
struct BroadcastAddXpuUtil final {
  OF_DEVICE_FUNC static void BackwardInputDiff(XpuVarNdarray<T>* in_diff,
                                               const XpuVarNdarray<const T>& out_diff,
                                               XpuVarNdarray<T>* tmp_storage) {
    XpuNdArrayBuilder<T, NDIMS> ndarray;
    const auto& out_diff_reduced = ndarray.Reduce(in_diff->shape(), out_diff, tmp_storage);
    in_diff->template AssignWithoutSyncThreads<NDIMS>(out_diff_reduced);
  }
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_KERNEL_BROADCAST_ADD_XPU_UTIL

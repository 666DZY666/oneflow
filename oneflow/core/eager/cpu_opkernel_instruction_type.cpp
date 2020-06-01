#include "oneflow/core/common/util.h"
#include "oneflow/core/job/job_desc.h"
#include "oneflow/core/eager/opkernel_object.h"
#include "oneflow/core/eager/blob_object.h"
#include "oneflow/core/eager/opkernel_instruction.msg.h"
#include "oneflow/core/eager/opkernel_instruction_type.h"
#include "oneflow/core/vm/string_object.h"
#include "oneflow/core/vm/stream.msg.h"
#include "oneflow/core/vm/cpu_stream_type.h"
#include "oneflow/core/vm/instruction.msg.h"
#include "oneflow/core/vm/object.h"

namespace oneflow {
namespace eager {

class CpuCallOpKernelInstructionType final : public CallOpKernelInstructionType {
 public:
  CpuCallOpKernelInstructionType() = default;
  ~CpuCallOpKernelInstructionType() override = default;

  using stream_type = vm::CpuStreamType;

 private:
  const char* device_tag() const override { return stream_type().device_tag(); }
};
COMMAND(vm::RegisterInstructionType<CpuCallOpKernelInstructionType>("cpu.CallOpKernel"));

class CpuStatelessCallOpKernelInstructionType final : public StatelessCallOpKernelInstructionType {
 public:
  CpuStatelessCallOpKernelInstructionType() = default;
  ~CpuStatelessCallOpKernelInstructionType() override = default;

  using stream_type = vm::CpuStreamType;

 private:
  const char* device_tag() const override { return stream_type().device_tag(); }
};
COMMAND(vm::RegisterInstructionType<CpuStatelessCallOpKernelInstructionType>(
    "cpu.compute.StatelessCallOpKernel"));

class CpuDeprecatedStatelessCallOpKernelInstructionType final
    : public DeprecatedStatelessCallOpKernelInstructionType {
 public:
  CpuDeprecatedStatelessCallOpKernelInstructionType() = default;
  ~CpuDeprecatedStatelessCallOpKernelInstructionType() override = default;

  using stream_type = vm::CpuStreamType;

 private:
  const char* device_tag() const override { return stream_type().device_tag(); }
};
COMMAND(vm::RegisterInstructionType<CpuDeprecatedStatelessCallOpKernelInstructionType>(
    "cpu.compute.DeprecatedStatelessCallOpKernel"));

class CpuWatchBlobHeaderInstructionType final : public WatchBlobHeaderInstructionType {
 public:
  CpuWatchBlobHeaderInstructionType() = default;
  ~CpuWatchBlobHeaderInstructionType() override = default;

  using stream_type = vm::CpuStreamType;

 private:
  const char* device_tag() const override { return stream_type().device_tag(); }
};
COMMAND(vm::RegisterInstructionType<CpuWatchBlobHeaderInstructionType>("cpu.WatchBlobHeader"));

class CpuWatchBlobBodyInstructionType final : public WatchBlobBodyInstructionType {
 public:
  CpuWatchBlobBodyInstructionType() = default;
  ~CpuWatchBlobBodyInstructionType() override = default;

  using stream_type = vm::CpuStreamType;

 private:
  const char* device_tag() const override { return stream_type().device_tag(); }
};
COMMAND(vm::RegisterInstructionType<CpuWatchBlobBodyInstructionType>("cpu.WatchBlobBody"));

}  // namespace eager
}  // namespace oneflow

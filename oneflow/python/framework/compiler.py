from __future__ import absolute_import

import oneflow.core.job.job_pb2 as job_util
import oneflow.python.lib.core.func_inspect_util as func_inspect_util
import oneflow.python.lib.core.pb_util as pb_util
import oneflow.python.framework.c_api_util as c_api_util
import oneflow.python.framework.compile_context as compile_context
import oneflow.python.framework.placement_util as placement_util
import oneflow.python.framework.config_util as config_util
import oneflow.python.framework.remote_blob as remote_blob_util
import oneflow.python.framework.input_blob_def as input_blob_def
import oneflow.python.framework.job_builder as job_builder
import oneflow.python.ops as ops
from oneflow.python.lib.core.box import Box

from contextlib import contextmanager

from oneflow.python.oneflow_export import oneflow_export

def Compile(job_func):
    job_conf = job_util.JobConfigProto()
    job_conf.job_name = job_func.__name__
    with compile_context.CurJobConf(job_conf), job_builder.JobBuildAndInferCtx(job_conf.job_name):
        _CompileJob(job_conf, job_func, config_util.default_config_proto)
        job_builder.CurCtxSetJobConfIfNotSet(job_conf)
        assert job_builder.CurCtxHasJobConf()
        job_builder.CurCtxCheckJob()

def _CompileJob(job_conf, func, config):
    device_type, machine_dev_ids = placement_util.GetDefaultMachineDeviceIds(config.resource)
    func.__oneflow_input_blob_defs__ = _GetArgDefault(func)
    def IsValidBlob(blob):
        return isinstance(blob, input_blob_def.input_blob_def) \
            or isinstance(blob, remote_blob_util.RemoteBlob)
    with placement_util.DevicePriorPlacementScope(device_type, machine_dev_ids):
        with _SetJobConfBeforeInferOp(job_conf) as set_job_conf:
            with _AddInputOpBeforeNonInputOp(func, set_job_conf): ret_remote_blobs = func()
        if ret_remote_blobs is None:
            func.__oneflow_output_remote_blobs__ = None 
        elif IsValidBlob(ret_remote_blobs):
            func.__oneflow_output_remote_blobs__ = ops.RetOpByRemoteBlob(ret_remote_blobs)
        elif isinstance(ret_remote_blobs, tuple) or isinstance(ret_remote_blobs, list):
            func.__oneflow_output_remote_blobs__ = []
            for remote_blob in ret_remote_blobs:
                assert IsValidBlob(remote_blob)
                output_remote_blob = ops.RetOpByRemoteBlob(remote_blob)
                func.__oneflow_output_remote_blobs__.append(output_remote_blob)
            if isinstance(ret_remote_blobs, tuple):
                func.__oneflow_output_remote_blobs__ = tuple(func.__oneflow_output_remote_blobs__)
        else:
            raise NotImplementedError

@contextmanager
def _SetJobConfBeforeInferOp(job_conf):
    job_conf_has_set = Box(False)
    def SetJobconf():
        if job_conf_has_set.value: return
        job_conf_has_set.set_value(True)
        config_util.TryCompleteDefaultJobConfigProto(job_conf)
        pb_util.MergePbMessage(job_conf, config_util.default_job_conf)
        job_builder.CurCtxSetJobConfIfNotSet(job_conf)
    with compile_context.BeforeNonInputOpBuildAndInferHook(SetJobconf):
        yield SetJobconf
    if job_conf_has_set.value == False: SetJobconf()

@contextmanager
def _AddInputOpBeforeNonInputOp(func, do_before_infer_input_op):
    input_op_add_and_infered = Box(False)
    def AddAndInferInputOp():
        if input_op_add_and_infered.value: return
        input_op_add_and_infered.set_value(True)
        do_before_infer_input_op()
        for blob_desc in func.__oneflow_input_blob_defs__:
            assert isinstance(blob_desc, input_blob_def.input_blob_def)
            ops.InputOpByBlobDesc(blob_desc)
    with compile_context.BeforeNonInputOpBuildAndInferHook(AddAndInferInputOp):
        yield None
    if input_op_add_and_infered.value == False: AddAndInferInputOp()

def _GetArgDefault(func):
    if hasattr(func, '__oneflow_arg_default__'): return func.__oneflow_arg_default__
    return func_inspect_util.GetArgDefaults(func)

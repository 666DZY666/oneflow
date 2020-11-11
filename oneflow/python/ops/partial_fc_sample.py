"""
Copyright 2020 The OneFlow Authors. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from __future__ import absolute_import

import os

import oneflow as flow
import oneflow.core.operator.op_conf_pb2 as op_conf_util
import oneflow.core.register.logical_blob_id_pb2 as logical_blob_id_util
import oneflow.python.framework.dtype as dtype_util
import oneflow.python.framework.distribute as distribute_util
import oneflow.python.framework.id_util as id_util
import oneflow.python.framework.remote_blob as remote_blob_util
from oneflow.python.oneflow_export import oneflow_export
from typing import Optional, Union


@oneflow_export("partial_fc_sample")
def partial_fc_sample(
    weight: remote_blob_util.BlobDef,
    label: remote_blob_util.BlobDef,
    num_sample: int,
    name: Optional[str] = None,
) -> remote_blob_util.BlobDef:

    return (
        flow.user_op_builder(
            name if name is not None else id_util.UniqueStr("PartialFcSample_")
        )
        .Op("partial_fc_sample")
        .Input("weight", [weight])
        .Input("label", [label])
        .Attr("num_sample", num_sample)
        .Output("maped_label")
        .Output("sampled_label")
        .Output("sampled_weight")
        .Build()
        .InferAndTryRun()
        .RemoteBlobList()
    )

/*
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
*/
#ifndef ONEFLOW_CORE_GRAPH_STREAM_INDEX_GETTER_REGISTRY_MANAGER_H_
#define ONEFLOW_CORE_GRAPH_STREAM_INDEX_GETTER_REGISTRY_MANAGER_H_

#include "oneflow/core/common/util.h"
#include "oneflow/core/graph/stream_index_getter_registry.h"

namespace oneflow {

template<typename Value>
using StreamIndexKeyMap = std::unordered_map<std::pair<DeviceType, TaskType>, Value, std::hash<std::pair<DeviceType, TaskType>>>;

class StreamIndexGetterRegistryManager final {
 private:
  StreamIndexGetterRegistryManager() {}

 public:
  OF_DISALLOW_COPY_AND_MOVE(StreamIndexGetterRegistryManager);

 public:
  static StreamIndexGetterRegistryManager& Get();

  StreamIndexKeyMap<StreamIndexGetterFn>& StreamIndexGetterFuncs();

 private:
  StreamIndexKeyMap<StreamIndexGetterFn> stream_index_getter_funcs_;
};

#define REGISTER_COMP_TASK_NODE_STREAM_INDEX_GETTER(device_type, task_type)                      \
  static ::oneflow::StreamIndexGetterRegistry OF_PP_CAT(g_strm_indx_get_registry, __COUNTER__) = \
      StreamIndexGetterRegistry(device_type, task_type)

}  // namespace oneflow
#endif
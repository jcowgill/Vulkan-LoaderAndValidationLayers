/* Copyright (c) 2015-2016 The Khronos Group Inc.
 * Copyright (c) 2015-2016 Valve Corporation
 * Copyright (c) 2015-2016 LunarG, Inc.
 * Copyright (C) 2015-2016 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Courtney Goeltzenleuchter <courtneygo@google.com>
 * Author: Tobin Ehlis <tobine@google.com>
 * Author: Chris Forbes <chrisf@ijw.co.nz>
 * Author: Mark Lobodzinski <mark@lunarg.com>
 */

// Check for noexcept support
#if defined(__clang__)
#if __has_feature(cxx_noexcept)
#define HAS_NOEXCEPT
#endif
#else
#if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46
#define HAS_NOEXCEPT
#else
#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026 && defined(_HAS_EXCEPTIONS) && _HAS_EXCEPTIONS
#define HAS_NOEXCEPT
#endif
#endif
#endif

#ifdef HAS_NOEXCEPT
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#pragma once
#include "core_validation_error_enums.h"
#include "core_validation_types.h"
#include "descriptor_sets.h"
#include "vulkan/vk_layer.h"
#include <atomic>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// fwd decls
namespace core_validation {
struct shader_module;
};
// TODO : Split this into separate structs for instance and device level data?
struct layer_data {
    VkInstance instance;

    debug_report_data *report_data;
    std::vector<VkDebugReportCallbackEXT> logging_callback;
    VkLayerDispatchTable *device_dispatch_table;
    VkLayerInstanceDispatchTable *instance_dispatch_table;

    devExts device_extensions;
    std::unordered_set<VkQueue> queues;  // all queues under given device
    // Global set of all cmdBuffers that are inFlight on this device
    std::unordered_set<VkCommandBuffer> globalInFlightCmdBuffers;
    // Layer specific data
    std::unordered_map<VkSampler, std::unique_ptr<SAMPLER_NODE>> samplerMap;
    std::unordered_map<VkImageView, VkImageViewCreateInfo> imageViewMap;
    std::unordered_map<VkImage, IMAGE_NODE> imageMap;
    std::unordered_map<VkBufferView, VkBufferViewCreateInfo> bufferViewMap;
    std::unordered_map<VkBuffer, BUFFER_NODE> bufferMap;
    std::unordered_map<VkPipeline, PIPELINE_NODE *> pipelineMap;
    std::unordered_map<VkCommandPool, CMD_POOL_INFO> commandPoolMap;
    std::unordered_map<VkDescriptorPool, DESCRIPTOR_POOL_NODE *> descriptorPoolMap;
    std::unordered_map<VkDescriptorSet, cvdescriptorset::DescriptorSet *> setMap;
    std::unordered_map<VkDescriptorSetLayout, cvdescriptorset::DescriptorSetLayout *> descriptorSetLayoutMap;
    std::unordered_map<VkPipelineLayout, PIPELINE_LAYOUT_NODE> pipelineLayoutMap;
    std::unordered_map<VkDeviceMemory, DEVICE_MEM_INFO> memObjMap;
    std::unordered_map<VkFence, FENCE_NODE> fenceMap;
    std::unordered_map<VkQueue, QUEUE_NODE> queueMap;
    std::unordered_map<VkEvent, EVENT_NODE> eventMap;
    std::unordered_map<QueryObject, bool> queryToStateMap;
    std::unordered_map<VkQueryPool, QUERY_POOL_NODE> queryPoolMap;
    std::unordered_map<VkSemaphore, SEMAPHORE_NODE> semaphoreMap;
    std::unordered_map<VkCommandBuffer, GLOBAL_CB_NODE *> commandBufferMap;
    std::unordered_map<VkFramebuffer, FRAMEBUFFER_NODE> frameBufferMap;
    std::unordered_map<VkImage, std::vector<ImageSubresourcePair>> imageSubresourceMap;
    std::unordered_map<ImageSubresourcePair, IMAGE_LAYOUT_NODE> imageLayoutMap;
    std::unordered_map<VkRenderPass, RENDER_PASS_NODE *> renderPassMap;
    std::unordered_map<VkShaderModule, std::unique_ptr<core_validation::shader_module>> shaderModuleMap;
    VkDevice device;

    // Device specific data
    PHYS_DEV_PROPERTIES_NODE phys_dev_properties;
    VkPhysicalDeviceMemoryProperties phys_dev_mem_props;

    layer_data()
        : report_data(nullptr), device_dispatch_table(nullptr), instance_dispatch_table(nullptr), device_extensions(),
          device(VK_NULL_HANDLE), phys_dev_properties{}, phys_dev_mem_props{} {};
};
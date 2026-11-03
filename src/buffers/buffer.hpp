#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vk_mem_alloc.h>

namespace renderer {
    class buffer {
        public:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags flags, VkBuffer& buffer, VmaAllocation& allocation);
        void destroyBuffer(VkBuffer buffer, VmaAllocation allocation);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    };
}
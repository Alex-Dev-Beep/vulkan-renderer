#include <memory/memory.hpp>
#include <app/app.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace renderer {
    void memoryAllocator::createAllocator() {
        
        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
        allocatorCreateInfo.physicalDevice = vulkan::physicalDevice.get();
        allocatorCreateInfo.device = vulkan::logicalDevice.get();
        allocatorCreateInfo.instance = vulkan::instance.get();
        
        vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    }

    VmaAllocator memoryAllocator::get() const {
        return allocator;
    }

    void memoryAllocator::checkVmaResult(VkResult result, const char* operation) {
        if (result != VK_SUCCESS) {
            throw std::runtime_error(
                std::string(operation) +
                " failed with VkResult: " +
                std::to_string(result)
            );
        }
    }
}
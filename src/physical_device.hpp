#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "logical_device.hpp"

#include <vector>

inline const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace renderer {
    class physicalDevice {
        public:

        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        
        VkPhysicalDevice get() const;
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        private:
        VkPhysicalDevice device;
    };

    class queues {
        public:
        inline static VkQueue graphicsQueue{};
        inline static VkQueue presentQueue{};
    };
}
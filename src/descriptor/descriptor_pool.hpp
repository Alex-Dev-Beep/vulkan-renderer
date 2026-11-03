#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class descriptorPool {
        public:
        void createDescriptorPool();
        VkDescriptorPool get() const;
        VkDescriptorPool getImguiPool() const;

        private:
        VkDescriptorPool vkDescriptorPool;
        VkDescriptorPool imguiPool;
    };
}
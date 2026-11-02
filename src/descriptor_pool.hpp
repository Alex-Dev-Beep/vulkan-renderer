#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class descriptorPool {
        public:
        void createDescriptorPool();
        VkDescriptorPool get() const;

        private:
        VkDescriptorPool vkDescriptorPool;
    };
}
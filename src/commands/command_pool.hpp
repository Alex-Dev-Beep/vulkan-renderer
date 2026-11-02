#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class commandPool {
        public:
        void createCommandPool();
        VkCommandPool get() const;

        private:
        VkCommandPool vkCommandPool;
    };
}
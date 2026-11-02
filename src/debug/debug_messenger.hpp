#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class debugMessenger {
        public:
        void setupDebugMessenger();

        VkDebugUtilsMessengerEXT get() const;

        private:
        VkDebugUtilsMessengerEXT debugMessenger;
    };
}
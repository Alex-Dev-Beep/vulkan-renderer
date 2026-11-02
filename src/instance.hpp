#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {

    class instance {
        public:

        void createInstance();
        VkInstance get() const;

        private:
        VkInstance vkInstance = VK_NULL_HANDLE;
    };

}
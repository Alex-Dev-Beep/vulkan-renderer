#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {

    class instance {
        public:
        instance();
        instance(const instance&) = delete;
        instance& operator=(const instance&) = delete;
        ~instance();

        void createInstance();
        VkInstance get() const;

        private:
        VkInstance vkInstance = VK_NULL_HANDLE;
    };

}
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class surface {
        public:

        VkSurfaceKHR get() const;

        void createSurface();

        private:
        VkSurfaceKHR surface;
    };
}
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class msaaSamples {
        public:

        static VkSampleCountFlagBits getMaxUsableSampleCount();
    };
}
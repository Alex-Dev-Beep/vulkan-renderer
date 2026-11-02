#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace renderer {
    class renderPass {
        public:

        VkRenderPass get() const;
        void createRenderPass();
        VkFormat findDepthFormat();
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        private:
        VkRenderPass vkRenderPass;
    };
}
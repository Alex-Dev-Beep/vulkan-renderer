#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace renderer {
    class graphicsPipeline {
        public:
        void createGraphicsPipeline();
        
        VkShaderModule createShaderModule(const std::vector<char>& code);
        VkPipeline get() const;
        VkPipelineLayout getLayout() const;

        private:
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
    };
}
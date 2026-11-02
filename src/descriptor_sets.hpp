#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer {
    class descriptorSetLayout {
        public:
        void createDescriptorSetLayout();
        const VkDescriptorSetLayout& get() const;

        private:
        VkDescriptorSetLayout descriptorSetLayout;
    };
}
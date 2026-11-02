#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace renderer {
    class descriptorSetLayout {
        public:
        void createDescriptorSetLayout();
        const VkDescriptorSetLayout& get() const;

        private:
        VkDescriptorSetLayout descriptorSetLayout;
    };

    class descriptorSets {
        public:
        void createDescriptorSets();
        std::vector<VkDescriptorSet> descriptorSets;

        private:
    };
}
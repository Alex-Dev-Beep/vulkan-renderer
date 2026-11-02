#pragma once

#include "image_views.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "render_pass.hpp"
#include "surface.hpp"
#include "swapchain.hpp"

class vulkan {
    public:
    inline static renderer::instance instance;
    inline static renderer::debugMessenger debugMessenger;
    inline static renderer::surface surface;
    inline static renderer::physicalDevice physicalDevice;
    inline static renderer::logicalDevice logicalDevice;
    inline static renderer::swapChain swapchain;
    inline static renderer::imageViews imageViews;
    inline static renderer::renderPass renderPass;
    inline static renderer::queues queues{};
    inline static VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};
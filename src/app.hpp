#pragma once

#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "image_views.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "render_pass.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "descriptor_sets.hpp"
#include "graphics_pipeline.hpp"
#include "image.hpp"
#include "resources.hpp"
#include "framebuffers.hpp"
#include "buffer.hpp"

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
    inline static renderer::descriptorSetLayout descriptorSetLayout;
    inline static renderer::graphicsPipeline graphicsPipeline;
    inline static renderer::commandPool commandPool;
    inline static renderer::image image;
    inline static renderer::resources resources;
    inline static renderer::framebuffers framebuffers;
    inline static renderer::buffer buffer;
    inline static renderer::commandBuffer commandBuffer;
    inline static renderer::queues queues{};
    inline static VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};
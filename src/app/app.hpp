#pragma once

#include "memory/memory.hpp"
class App {
    public:
    void run();
    void initVulkan();
    void mainLoop();
};

#include <commands/command_buffer.hpp>
#include <commands/command_pool.hpp>
#include <descriptor/descriptor_pool.hpp>
#include <images/image_views.hpp>
#include <instance/instance.hpp>
#include <debug/debug_messenger.hpp>
#include <device/logical_device.hpp>
#include <device/physical_device.hpp>
#include <pipeline/render_pass.hpp>
#include <surface/surface.hpp>
#include <swapchain/swapchain.hpp>
#include <descriptor/descriptor_sets.hpp>
#include <pipeline/graphics_pipeline.hpp>
#include <images/image.hpp>
#include <images/resources.hpp>
#include <renderer/framebuffers.hpp>
#include <buffers/buffer.hpp>
#include <buffers/uniform_buffers.hpp>

#include <vk_mem_alloc.h>

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
    inline static renderer::descriptorPool descriptorPool;
    inline static renderer::uniformBuffers uniformBuffers;
    inline static renderer::descriptorSets descriptorSets;
    inline static renderer::memoryAllocator memoryAllocator;
    inline static VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    inline static std::vector<VkSemaphore> imageAvailableSemaphores;
    inline static std::vector<VkSemaphore> renderFinishedSemaphores;
    inline static std::vector<VkFence> inFlightFences;
    inline static const int MAX_FRAMES_IN_FLIGHT = 2;
    inline static uint32_t currentFrame = 0;
};
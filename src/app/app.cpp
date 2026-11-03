#include <app/app.hpp>
#include <app/cleanup.hpp>
#include <mesh/model.hpp>
#include <texture/texture.hpp>
#include <sync/sync.hpp>
#include <window/window.hpp>
#include <renderer/renderer.hpp>
#include <imgui/imgui_integration.hpp>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

void App::run() {
    initVulkan();
    mainLoop();
    renderer::cleanup();
}

void App::initVulkan() {
    vulkan::instance.createInstance();
    vulkan::debugMessenger.setupDebugMessenger();
    vulkan::surface.createSurface();
    vulkan::physicalDevice.pickPhysicalDevice();
    vulkan::logicalDevice.createLogicalDevice();
    vulkan::swapchain.createSwapChain();
    vulkan::imageViews.createImageViews();
    vulkan::renderPass.createRenderPass();
    vulkan::descriptorSetLayout.createDescriptorSetLayout();
    vulkan::graphicsPipeline.createGraphicsPipeline();
    vulkan::commandPool.createCommandPool();
    vulkan::resources.createColorResources();
    vulkan::resources.createDepthResources();
    vulkan::framebuffers.createFramebuffers();

    renderer::texture vikingRoomTexture;
    vikingRoomTexture.path = "assets/textures/viking_room.png";

    renderer::createTexture(vikingRoomTexture);
        
    renderer::mesh vikingRoom;
    vikingRoom.path = "assets/models/viking_room.obj";
    
    renderer::createModel(vikingRoom);
    renderer::meshes.push_back(vikingRoom);

    vulkan::uniformBuffers.createUniformBuffers();
    vulkan::descriptorPool.createDescriptorPool();
    vulkan::descriptorSets.createDescriptorSets();
    vulkan::commandBuffer.createCommandBuffers();
    renderer::createSyncObjects();

    initializeImGuiVulkan();
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(renderer::mainWindow.get())) {
        glfwPollEvents();

        renderer::drawFrame();
    }

    vkDeviceWaitIdle(vulkan::logicalDevice.get());
}
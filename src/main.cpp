#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include "window.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "objects.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "image_views.hpp"
#include "render_pass.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "cleanup.hpp"
#include "sync.hpp"
#include "renderer.hpp"

class App {
public:
    void run() {
        initVulkan();
        mainLoop();
        renderer::cleanup();
    }

private:

    void initVulkan() {
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
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(renderer::mainWindow.get())) {
            glfwPollEvents();
            renderer::drawFrame();
        }

        vkDeviceWaitIdle(vulkan::logicalDevice.get());
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }
};

int main() {
    App app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

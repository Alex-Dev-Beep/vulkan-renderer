#include <imgui/imgui_integration.hpp>
#include <window/window.hpp>
#include <instance/instance.hpp>
#include <app/app.hpp>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <cstdio>
#include <cstdlib>

void checkVulkanResult(VkResult err) {
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

void initializeImGuiVulkan() {
    QueueFamilyIndices indices = vulkan::physicalDevice.findQueueFamilies(vulkan::physicalDevice.get());
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(renderer::mainWindow.get(), true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = vulkan::instance.get();
    initInfo.PhysicalDevice = vulkan::physicalDevice.get();
    initInfo.Device = vulkan::logicalDevice.get();
    initInfo.QueueFamily = indices.graphicsFamily.value();
    initInfo.Queue = vulkan::queues.graphicsQueue;
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = vulkan::descriptorPool.getImguiPool();
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = 2;
    initInfo.Allocator = nullptr;
    initInfo.PipelineInfoMain.RenderPass = vulkan::renderPass.get();
    initInfo.PipelineInfoMain.Subpass = 0;
    initInfo.PipelineInfoMain.MSAASamples = vulkan::msaaSamples;
    initInfo.CheckVkResultFn = checkVulkanResult;
    ImGui_ImplVulkan_Init(&initInfo);
}
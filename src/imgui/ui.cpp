#include "ui.hpp"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

void draw()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Window");

    static int selected = 0;

    const char* items[] = {
        "Object 0",
        "Object 1",
        "Object 2",
        "Object 3",
        "Object 4",
        "Object 5",
        "Object 6",
        "Object 7",
        "Object 8",
        "Object 9"
    };

    ImGui::Combo("Objects", &selected, items, IM_ARRAYSIZE(items));

    ImGui::End();

    ImGui::Render();
}
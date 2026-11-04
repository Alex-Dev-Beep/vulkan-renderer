#include "ui.hpp"

#include <stdio.h>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <imgui/ui.hpp>

void draw()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Window");

    static ImGuiSelectionBasicStorage selection;

    constexpr int ITEMS_COUNT = 10;

    char preview[64];

    snprintf(preview, sizeof(preview), "%d selected", selection.Size);

    if (ImGui::BeginCombo("Objects", preview))
    {
        ImGuiMultiSelectFlags flags =
            ImGuiMultiSelectFlags_ClearOnEscape;

        ImGuiMultiSelectIO* ms_io =
            ImGui::BeginMultiSelect(flags, selection.Size, ITEMS_COUNT);

        selection.ApplyRequests(ms_io);

        for (int i = 0; i < ITEMS_COUNT; ++i)
        {
            char label[32];
            snprintf(label, sizeof(label), "Object %d", i);

            bool selected = selection.Contains(static_cast<ImGuiID>(i));

            ImGui::SetNextItemSelectionUserData(i);

            if (ImGui::Selectable(label, selected))
            {
                // La selección la gestiona MultiSelect.
            }
        }

        ms_io = ImGui::EndMultiSelect();

        selection.ApplyRequests(ms_io);

        ImGui::EndCombo();
    }

    ImGui::End();

    ImGui::Render();
}
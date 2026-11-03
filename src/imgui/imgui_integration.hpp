#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void checkVulkanResult(VkResult err);
void initializeImGuiVulkan();
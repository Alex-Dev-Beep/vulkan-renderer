#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "model.hpp"

namespace renderer {
    void createVertexBuffer(mesh& model);
}
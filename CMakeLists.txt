cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Renderer)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(Vulkan REQUIRED)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)

# Dear ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp

    # GLFW backend
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp

    # Vulkan backend
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC
    glfw
    Vulkan::Vulkan
)

add_executable(renderer
    src/main.cpp
    src/debug/validation_layers.hpp
    src/debug/validation_layers.cpp
    src/window/window.hpp
    src/window/window.cpp
    src/instance/instance.hpp
    src/instance/instance.cpp
    src/debug/debug_messenger.cpp
    src/debug/debug_messenger.hpp
    src/surface/surface.hpp
    src/surface/surface.cpp
    src/device/physical_device.hpp
    src/device/physical_device.cpp
    src/device/logical_device.cpp
    src/device/logical_device.hpp
    src/swapchain/swapchain.hpp
    src/swapchain/swapchain.cpp
    src/renderer/msaaSamples.hpp
    src/renderer/msaaSamples.cpp
    src/images/image_views.hpp
    src/images/image_views.cpp
    src/pipeline/render_pass.hpp
    src/pipeline/render_pass.cpp
    src/descriptor/descriptor_sets.cpp
    src/descriptor/descriptor_sets.hpp
    src/pipeline/graphics_pipeline.cpp
    src/pipeline/graphics_pipeline.hpp
    src/vertex.hpp
    src/utilities/filesystem.hpp
    src/utilities/filesystem.cpp
    src/commands/command_pool.cpp
    src/commands/command_pool.hpp
    src/images/resources.hpp
    src/images/resources.cpp
    src/images/image.hpp
    src/images/image.cpp
    src/texture/texture.hpp
    src/texture/texture.cpp
    src/renderer/framebuffers.cpp
    src/renderer/framebuffers.hpp
    src/implementation.cpp
    src/buffers/buffer.hpp
    src/buffers/buffer.cpp
    src/commands/command_buffer.cpp
    src/commands/command_buffer.hpp
    src/mesh/model.hpp
    src/mesh/model.cpp
    src/buffers/vertex_buffer.hpp
    src/buffers/vertex_buffer.cpp
    src/buffers/index_buffer.cpp
    src/buffers/index_buffer.hpp
    src/app/cleanup.cpp
    src/app/cleanup.hpp
    src/buffers/uniform_buffers.hpp
    src/buffers/uniform_buffers.cpp
    src/descriptor/descriptor_pool.cpp
    src/descriptor/descriptor_pool.hpp
    src/sync/sync.hpp
    src/sync/sync.cpp
    src/renderer/renderer.hpp
    src/renderer/renderer.cpp
    src/app/app.cpp
    src/app/app.hpp
    src/imgui/imgui_integration.hpp
    src/imgui/imgui_integration.cpp
)

target_link_libraries(renderer PRIVATE
    glfw
    glm
    Vulkan::Vulkan
    imgui
)

target_include_directories(renderer PRIVATE
    thirdparty/model_loading
    thirdparty/stb
    thirdparty/glm
    thirdparty/glfw/include
    include
    src
)

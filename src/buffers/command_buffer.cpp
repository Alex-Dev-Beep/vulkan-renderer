#include <commands/command_buffer.hpp>
#include <app/app.hpp>
#include <model_loading/model.hpp>

#include <stdexcept>

namespace renderer {
    VkCommandBuffer commandBuffer::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkan::commandPool.get();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer{};

        VkResult result = vkAllocateCommandBuffers(
            vulkan::logicalDevice.get(),
            &allocInfo,
            &commandBuffer
        );

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffer");
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin command buffer");
        }

        return commandBuffer;
    }

    void commandBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vulkan::queues.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vulkan::queues.graphicsQueue);

        vkFreeCommandBuffers(vulkan::logicalDevice.get(), vulkan::commandPool.get(), 1, &commandBuffer);
    }

    void commandBuffer::createCommandBuffers() {
        commandBuffers.resize(vulkan::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vulkan::commandPool.get();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(vulkan::logicalDevice.get(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
    
    void commandBuffer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkan::renderPass.get();
        renderPassInfo.framebuffer = vulkan::swapchain.getFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vulkan::swapchain.getExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            vulkan::graphicsPipeline.get()
        );

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkan::swapchain.getExtent().width);
        viewport.height = static_cast<float>(vulkan::swapchain.getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = vulkan::swapchain.getExtent();

        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        for (const auto& mesh : renderer::meshes) {
            VkBuffer vertexBuffers[] = {mesh.vertexBuffer};
            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(
                commandBuffer, 0, 1, vertexBuffers, offsets
            );

            

            vkCmdBindIndexBuffer(
                commandBuffer,
                mesh.indexBuffer,
                0,
                VK_INDEX_TYPE_UINT32
            );

            vkCmdBindDescriptorSets(
                commandBuffer, 
                VK_PIPELINE_BIND_POINT_GRAPHICS, 
                vulkan::graphicsPipeline.getLayout(), 
                0, 
                1,
                &vulkan::descriptorSets.descriptorSets[vulkan::currentFrame], 
                0, 
                nullptr
            );

            vkCmdDrawIndexed(
                commandBuffer,
                static_cast<uint32_t>(mesh.indices.size()),
                1,
                0,
                0,
                0
            );
        }

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

}
#pragma once

#include <vk_mem_alloc.h>

namespace renderer {
    class memoryAllocator {
        public:
        void createAllocator();
        void checkVmaResult(VkResult result, const char* operation);
        VmaAllocator get() const;

        private:
        VmaAllocator allocator;
    };
}
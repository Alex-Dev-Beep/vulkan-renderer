#pragma once

#include <vector>
#include <string>

namespace renderer {
    class filesystem {
        public:
        static std::vector<char> readFile(const std::string& filename);
        
    };
}
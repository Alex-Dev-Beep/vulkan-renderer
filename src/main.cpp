#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include <app/app.hpp>

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

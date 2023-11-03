#include "pch.hpp"
#include "renderer.hpp"


int main() {
    Renderer rd(1600, 1000, "Calculator");
    if (!rd.init()) {
        return EXIT_FAILURE;
    }
    
    rd.run();

    return EXIT_SUCCESS;
}
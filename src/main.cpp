#include "renderer.hpp"

int main() {
    Renderer rd(1800, 1200, "Calculator");
    if (!rd.init()) {
        return EXIT_FAILURE;
    }
    
    rd.run();

    return EXIT_SUCCESS;
}
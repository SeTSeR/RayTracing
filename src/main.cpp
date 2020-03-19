#include "Render.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
        auto config = parse_args(argc, argv);
        if (config) {
                Render render;
                render.renderImage(*config);
        } else {
                std::cout << "Usage:" << std::endl;
                std::cout << "RayTracing -out <output_path> -scene <scene_number> [-threads <threads_number> -background <background_image_path>]" << std::endl;
        }
        return 0;
}

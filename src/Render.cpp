#include <Image.hpp>
#include <Render.hpp>
#include <Vec3.hpp>

#include <vector>

void render(int width, int height, const std::filesystem::path &path) {
        Image framebuffer(width, height);

        for (size_t j = 0; j < height; ++j) {
                for (size_t i = 0; i < width; ++i) {
                        framebuffer[j][i] = Vec3(j/float(height), i/float(width), 0.f);
                }
        }

        framebuffer.write_png(path);
}

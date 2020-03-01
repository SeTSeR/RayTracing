#include <Image.hpp>

#include <fstream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_WRITE_NO_STDIO

#include <stb_image_write.h>

void write_func(void *context, void* data, int size) {
        auto out = static_cast<std::ofstream*>(context);
        out->write(static_cast<const char *>(data), size);
}

int Image::write_png(const std::filesystem::path& path) {
        const int RGB = 3;
        std::ofstream out(path);
        char* data = new char[width * height * RGB];
        for (ssize_t i = 0; i < height * width; ++i) {
                for (ssize_t j = 0; j < RGB; ++j) {
                        data[i*3 + j] = 255 * std::max(0.f, std::min(1.f, buffer[i][j]));
                }
        }
        int stride = 0;
        return stbi_write_png_to_func(&write_func, &out,
                                      width, height,
                                      RGB, data, stride);
}

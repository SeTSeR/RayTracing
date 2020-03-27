#include "Image.hpp"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_WRITE_NO_STDIO

#include <stb_image.h>
#include <stb_image_write.h>

int read(void *user, char* data, int size) {
        auto in = static_cast<std::ifstream*>(user);
        in->read(data, size);
        return in->gcount();
}

void skip(void *user, int n) {
        auto in = static_cast<std::ifstream*>(user);
        in->seekg(n, std::ios_base::cur);
}

int eof(void *user) {
        auto in = static_cast<std::ifstream*>(user);
        return in->eof();
}

int Image::read_image(const std::filesystem::path& path) {
        int x, y, n;
        const int RGB = 3;
        stbi_io_callbacks callbacks = {
                                       .read = &read,
                                       .skip = &skip,
                                       .eof = &eof,
        };
        std::ifstream in(path, std::ifstream::binary);
        unsigned char *data = stbi_load_from_callbacks(&callbacks, &in, &x, &y, &n, 0);
        if (data && n == RGB && x >= width && y >= height) {
                for (ssize_t i = 0; i < height * width; ++i) {
                        for (ssize_t j = 0; j < RGB; ++j) {
                                buffer[i][j] = data[i*RGB + j] / 255.f;
                        }
                }
        }
        stbi_image_free(data);
        return 0;
}

void write_func(void *context, void* data, int size) {
        auto out = static_cast<std::ofstream*>(context);
        out->write(static_cast<const char *>(data), size);
}

int Image::write_png(const std::filesystem::path& path) {
        const int RGB = 3;
        std::ofstream out(path, std::ofstream::binary);
        auto* data = new unsigned char[width * height * RGB];
        for (ssize_t i = 0; i < height * width; ++i) {
                for (ssize_t j = 0; j < RGB; ++j) {
                        data[i*RGB + j] = 255 * std::max(0.f, std::min(1.f, buffer[i][j]));
                }
        }
        int stride = 0;
        return stbi_write_png_to_func(&write_func, &out,
                                      width, height,
                                      RGB, data, stride);
}

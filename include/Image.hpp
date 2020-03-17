#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "Vec.hpp"

#include <filesystem>
#include <vector>

class Image {
private:
        int width;
        int height;
        std::vector<Vec<3, float>> buffer;
        class Proxy {
                int i;
                int width;
                std::vector<Vec<3, float>> &buffer;
        public:
                Proxy(int i, int width, std::vector<Vec<3, float>> &buffer): i(i), width(width), buffer(buffer) {};
                Vec<3, float> &operator[](int j) {
                        return buffer[i * width + j];
                }
        };
public:
        Image(int width, int height): width(width), height(height), buffer(width * height) {};
        Proxy operator[](int i) {
                return Proxy(i, width, buffer);
        };
        int write_png(const std::filesystem::path& path);
};

#endif

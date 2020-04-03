#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <filesystem>
#include <optional>

struct Config {
        std::optional<std::filesystem::path> background_path;
        std::filesystem::path output_path;
        std::optional<size_t> threads_num;
        size_t scene_num;
        size_t width = 1056;
        size_t height = 594;
        size_t BLOCK_LENGTH = 2*width;
        size_t BLOCK_COUNT = width * height / BLOCK_LENGTH;
        int MIN_SAMPLES = 10;
        float ERROR = 1;
        int MAX_DEPTH = 10;
        int SAMPLES_COUNT = 2;
};

std::optional<Config> parse_args(int argc, char* argv[]);

#endif

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
        int MIN_SAMPLES = 40;
        float ERROR = 0.5;
        int MAX_DEPTH = 10;
        int SAMPLES_COUNT = 2;
};

std::optional<Config> parse_args(int argc, char* argv[]);

#endif

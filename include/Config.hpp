#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <filesystem>
#include <optional>

struct Config {
        std::filesystem::path background_path;
        std::filesystem::path output_path;
        int threads_num;
        int scene_num;
        int width = 1024;
        int height = 768;
};

std::optional<Config> parse_args(int argc, char* argv[]);

#endif

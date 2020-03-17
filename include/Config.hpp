#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <filesystem>
#include <optional>

struct Config {
        std::optional<std::filesystem::path> background_path;
        std::filesystem::path output_path;
        std::optional<int> threads_num;
        int scene_num;
        int width = 1056;
        int height = 594;
};

std::optional<Config> parse_args(int argc, char* argv[]);

#endif

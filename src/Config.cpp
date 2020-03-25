#include "Config.hpp"

#include <cstring>

bool streq(const char *s1, const char *s2) {
        return (strlen(s1) == strlen(s2)) &&
                (strcmp(s1, s2) == 0);
}

std::optional<Config> parse_args(int argc, char* argv[]) {
        Config config = {};
        bool out_met = false;
        bool scene_met = false;
        for (int param_num = 1; param_num < argc; param_num += 2) {
                if (streq("-out", argv[param_num])) {
                        out_met = true;
                        config.output_path = argv[param_num + 1];
                } else if (streq("-scene", argv[param_num])) {
                        scene_met = true;
                        config.scene_num = atoi(argv[param_num + 1]);
                } else if (streq("-threads", argv[param_num])) {
                        config.threads_num = atoi(argv[param_num + 1]);
                } else if (streq("-background", argv[param_num])) {
                        config.background_path = argv[param_num + 1];
                }
        }
        if (out_met && scene_met) {
                return config;
        }
        return std::nullopt;
}

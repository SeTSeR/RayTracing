#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include "Config.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec.hpp"

#include <filesystem>
#include <random>
#include <vector>

class Render {
private:
        enum {
            RAY_TRACER = 0,
            PATH_TRACER = 1,
        } render_mode;
        Config config;
        std::vector<Scene<float>> scenes;
        Vec<3, float> castRay(const Ray<float>&, const Scene<float>&, const Vec<3, float>&, int depth = 0);
        Vec<3, float> tracePath(const Ray<float>&, const Scene<float>&, const Vec<3, float>&, int depth = 0);
        std::random_device rd = std::random_device();
        std::mt19937 gen = std::mt19937(rd());
        std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0, 1);
public:
        explicit Render(const Config &config);
        void renderImage();
};

#endif

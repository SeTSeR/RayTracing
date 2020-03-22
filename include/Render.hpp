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
        constexpr static int MIN_SAMPLES = 40;
        constexpr static float ERROR = 0.5;
        constexpr static int MAX_DEPTH = 10;
        constexpr static int SAMPLES_COUNT = 2;
        std::vector<Scene<float>> scenes;
        enum {
              RAY_TRACER = 0,
              PATH_TRACER = 1,
        } render_mode;
        Vec<3, float> castRay(const Ray<float>&, const Scene<float>&, const Vec<3, float>&, int depth = 0);
        Vec<3, float> tracePath(const Ray<float>&, const Scene<float>&, const Vec<3, float>&, int depth = 0);
        std::random_device rd = std::random_device();
        std::mt19937 gen = std::mt19937(rd());
        std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0, 1);
public:
        Render();
        void renderImage(const Config &config);
};

#endif

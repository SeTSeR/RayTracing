#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include "Config.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec.hpp"

#include <filesystem>
#include <vector>

class Render {
private:
        constexpr static int MAX_DEPTH = 4;
        constexpr static int SAMPLES_COUNT = 2;
        std::vector<Scene<float>> scenes;
        enum {
              RAY_TRACER = 0,
              PATH_TRACER = 1,
        } render_mode;
        Vec<3, float> castRay(const Ray<float>&, const Scene<float>&, const Vec<3, float>&, int depth = 0);
        Vec<3, float> tracePath(const Ray<float>&, const Scene<float>&, int depth = 0);
public:
        Render();
        void renderImage(const Config &config);
};

#endif

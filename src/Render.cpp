#include <Image.hpp>
#include <Render.hpp>
#include <Sphere.hpp>
#include <Vec3.hpp>
#include "Material.hpp"
#include "Scene.hpp"

#include <cmath>
#include <memory>
#include <vector>

Vec3<float> cast_ray(const Vec3<float> &origin, const Vec3<float> &direction, const Scene<float> &scene) {
        Vec3<float> point, norm;
        Material<float> material;
        if (!scene.intersects(origin, direction, point, norm, material)) {
                return Vec3(0.2f, 0.7f, 0.8f);
        }
        return material.getColor();
}

void render(int width, int height, const std::filesystem::path &path) {
        Image framebuffer(width, height);

        float fov = M_PI/2;
        Scene<float> scene;
        auto sphere = std::make_unique<Sphere<float>>(Vec3<float>(-3.f, 0.f, -16.f), 2.f, Material<float>::ivory());
        scene.addShape(std::move(sphere));

        for (size_t j = 0; j < height; ++j) {
                for (size_t i = 0; i < width; ++i) {
                        float x = (2*(i + 0.5)/(float)width - 1)*tan(fov/2.)*width/(float)height;
                        float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
                        Vec3<float> dir = Vec3<float>(x, y, -1).normalize();
                        framebuffer[j][i] = cast_ray(Vec3<float>(0, 0, 0), dir, scene);
                }
        }

        framebuffer.write_png(path);
}

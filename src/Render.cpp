#include "Image.hpp"
#include "Material.hpp"
#include "Plane.hpp"
#include "Render.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec.hpp"

#include <cmath>
#include <memory>
#include <vector>

Vec<3, float> cast_ray(const Vec<3, float> &origin, const Vec<3, float> &direction, const Scene<float> &scene) {
        Vec<3, float> point, norm;
        Material<float> material;
        if (!scene.intersects(origin, direction, point, norm, material)) {
                return Vec<3, float>(0.2f, 0.7f, 0.8f);
        }
        return scene.light_color(point, direction, norm, material);
}

void render(int width, int height, const std::filesystem::path &path) {
        Image framebuffer(width, height);

        float fov = M_PI/2;
        Scene<float> scene;
        scene.addShape(std::make_unique<Sphere<float>>(Vec<3, float>(-3.f, 0.f, -16.f), 2.f, Material<float>::ivory()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec<3, float>(-1.f, -1.5f, -12.f), 2.f, Material<float>::red_rubber()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec<3, float>(1.5f, -0.5f, -18.f), 3.f, Material<float>::red_rubber()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec<3, float>(7.f, 5.f, -18.f), 4.f, Material<float>::red_rubber()));
        scene.addShape(std::make_unique<Plane<float>>(Vec<3, float>(0.f, 1.f, 0.f), Vec<3, float>(-20.f, -20.f, -20.f), Material<float>::ivory()));
        scene.addLight(Light(Vec<3, float>(-20.f, 20.f, 20.f), 1.5f));
        scene.addLight(Light(Vec<3, float>(30.f, 50.f, -25.f), 1.8f));
        scene.addLight(Light(Vec<3, float>(30.f, 20.f, 30.f), 1.7f));
        
        for (size_t j = 0; j < height; ++j) {
                for (size_t i = 0; i < width; ++i) {
                        float x = (2*(i + 0.5)/(float)width - 1)*tan(fov/2.)*width/(float)height;
                        float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
                        Vec<3, float> dir = Vec<3, float>({x, y, -1.f}).normalize();
                        framebuffer[j][i] = cast_ray(Vec<3, float>{0.f, 0.f, 0.f}, dir, scene);
                        float max = std::max(framebuffer[j][i][0], std::max(framebuffer[j][i][1], framebuffer[j][i][2]));
                        if (max > 1) framebuffer[j][i] *= (1./max);
                }
        }

        framebuffer.write_png(path);
}

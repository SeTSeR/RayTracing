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

constexpr int MAX_DEPTH = 4;
constexpr int SAMPLES_COUNT = 2;

Vec<3, float> cast_ray(const Vec<3, float> &origin, const Vec<3, float> &direction, const Scene<float> &scene, int depth = 0) {
        Vec<3, float> point, norm;
        Material<float> material;
        if (depth > MAX_DEPTH || !scene.intersects(origin, direction, point, norm, material)) {
                return Vec<3, float>(0.2f, 0.7f, 0.8f);
        }
        Vec reflect_direction = direction.reflect(norm).normalize();
        Vec reflect_origin = reflect_direction * norm < 0 ? point - norm * 1e-3 : point + norm * 1e-3;
        Vec reflect_color = cast_ray(reflect_origin, reflect_direction, scene, depth + 1);

        std::optional refract_direction = direction.refract(norm, material.getRefractiveIndex())->normalize();
        if (refract_direction) {
                Vec refract_origin = *refract_direction * norm < 0 ? point - norm * 1e-3 : point + norm * 1e-3;
                Vec refract_color = cast_ray(refract_origin, *refract_direction, scene, depth + 1);
                return scene.light_color(point, direction, norm, material, reflect_color, refract_color);
        } else {
                return scene.light_color(point, direction, norm, material, reflect_color, Vec(0.f, 0, 0));
        }
}

void render(const Config &config) {
        Image framebuffer(config.width, config.height);

        float fov = M_PI/2;
        Scene<float> scene;
        scene.addShape(std::make_unique<Sphere<float>>(Vec(-3.f, 0, -16), 2.f, Material<float>::ivory()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec(-1.f, -1.5, -12), 2.f, Material<float>::glass()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec(1.5f, -0.5, -18), 3.f, Material<float>::red_rubber()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec(7.f, 5, -18), 4.f, Material<float>::mirror()));
        scene.addShape(std::make_unique<Plane<float>>(Vec(0.f, 1, 0), Vec(-1.f, -4, -12), Material<float>::ivory()));
        scene.addLight(Light(Vec(-20.f, 20, 20), 1.5f));
        scene.addLight(Light(Vec(30.f, 50, -25), 1.8f));
        scene.addLight(Light(Vec(30.f, 20, 30), 1.7f));
        
        for (size_t j = 0; j < config.height; ++j) {
                for (size_t i = 0; i < config.width; ++i) {
                        Vec<3, float> cell_color = {};
                        for (size_t k = 0; k < SAMPLES_COUNT; ++k) {
                                for (size_t l = 0; l < SAMPLES_COUNT; ++l) {
                                        float x = (2*(i*SAMPLES_COUNT + k + 0.5)/(float)(config.width*SAMPLES_COUNT) - 1)*tan(fov/2.)*config.width/(float)config.height;
                                        float y = -(2*(j*SAMPLES_COUNT + l + 0.5)/(float)(config.height*SAMPLES_COUNT) - 1)*tan(fov/2.);
                                        Vec<3, float> dir = Vec(x, y, -1).normalize();
                                        cell_color += cast_ray(Vec(0.f, 0, 0), dir, scene);
                                }
                        }
                        framebuffer[j][i] = cell_color * (1.f/(SAMPLES_COUNT*SAMPLES_COUNT));
                        float max = std::max(framebuffer[j][i][0], std::max(framebuffer[j][i][1], framebuffer[j][i][2]));
                        if (max > 1) framebuffer[j][i] *= (1./max);
                }
        }

        framebuffer.write_png(config.output_path);
}

#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "Light.hpp"
#include "Shape.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

template<typename T>
class Scene {
private:
        std::vector<Light<T>> lights;
        std::vector<std::unique_ptr<Shape<T>>> scene;
public:
        void addLight(Light<T> &&light) {
                lights.push_back(std::move(light));
        }
        void addShape(std::unique_ptr<Shape<T>> shape) {
                scene.push_back(std::move(shape));
        }
        bool intersects(const Vec<3, T> &origin, const Vec<3, T> &direction,
                        Vec<3, T> &hit, Vec<3, T> &norm, Material<T> &material) const {
                T dist = std::numeric_limits<T>::max();
                for (auto &shape: scene) {
                        T figure_dist;
                        if (shape->intersects(origin, direction, figure_dist) && figure_dist < dist) {
                                dist = figure_dist;
                                hit = origin + direction * figure_dist;
                                norm = shape->getNorm(hit);
                                material = shape->getMaterial();
                        }
                }
                return dist < 1000;
        }
        Vec<3, T> light_color(const Vec<3, T> &point,
                              const Vec<3, T> &direction,
                              const Vec<3, T> &norm,
                              const Material<T> &material) const {
                T diffuse_light_intensity = {};
                T specular_light_intensity = {};
                for (const auto &light : lights) {
                        Vec light_direction = (light.getPosition() - point).normalize();
                        Vec shadow_orig = light_direction * norm < 0 ? point - norm * 1e-3 : point + norm * 1e-3;
                        Vec shadow_direction = (light.getPosition() - point).normalize();
                        Material<T> mat;
                        Vec<3, T> hit, n;
                        if (!intersects(shadow_orig, shadow_direction, hit, n, mat)) {
                                diffuse_light_intensity += light.getIntensity() * std::max(0.f, light_direction * norm);
                                specular_light_intensity += light.getIntensity() * std::pow(std::max(0.f, light_direction.reflect(norm) * direction), material.getSpecularExponent());
                        }
                }
                return material.getDiffuseColor() * diffuse_light_intensity * material.getAlbedo()[0] + Vec<3, float>(1.f, 1.f, 1.f) * specular_light_intensity * material.getAlbedo()[1];
        }
};

#endif

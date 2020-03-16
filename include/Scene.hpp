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
        bool intersects(const Vec3<T> &origin, const Vec3<T> &direction,
                        Vec3<T> &hit, Vec3<T> &norm, Material<T> &material) const {
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
        Vec3<T> light_color(const Vec3<T> &point,
                      const Vec3<T> &norm,
                      const Material<T> &material) const {
                T diffuse_light_intensity = {};
                for (const auto &light : lights) {
                        Vec3<T> light_direction = (light.getPosition() - point).normalize();
                        diffuse_light_intensity += light.getIntensity() * std::max(0.f, light_direction * norm);
                }
                return material.getColor() * diffuse_light_intensity;
        }
};

#endif

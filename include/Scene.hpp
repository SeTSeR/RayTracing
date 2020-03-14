#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "Shape.hpp"

#include <memory>
#include <vector>

template<typename T>
class Scene {
private:
        std::vector<std::unique_ptr<Shape<T>>> scene;
public:
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
};

#endif

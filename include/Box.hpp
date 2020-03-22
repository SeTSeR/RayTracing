#ifndef _BOX_HPP
#define _BOX_HPP

#include "Plane.hpp"
#include "Shape.hpp"
#include "Vec.hpp"

#include <tuple>

template<typename T>
class Box: public Shape<T> {
private:
        const static ssize_t MCOUNT = 6;
        std::array<Plane<T>, MCOUNT> margins;
        Material<T> material;
public:
        Box(const Vec<3, T> &center, const Vec<3, T> &corner_dir, const Vec<3, T> &norm, const Material<T> &material): material(material) {
                auto corner = center + corner_dir;
                auto opposite_corner = center - corner_dir;
                auto norms = Vec<3, float>::genOrthogonal(norm, corner_dir);
                margins[0] = Plane(norm, corner, material);
                margins[1] = Plane(norms.first, corner, material);
                margins[2] = Plane(norms.second, corner, material);
                margins[3] = Plane(-norm, opposite_corner, material);
                margins[4] = Plane(-norms.first, opposite_corner, material);
                margins[5] = Plane(-norms.second, opposite_corner, material);
        }
        virtual const Material<T> &getMaterial() const {
                return material;
        }
        virtual bool intersects(const Ray<T> &ray, T& distance) const {
                T margin_distance = std::numeric_limits<T>::infinity();
                bool intersects_any = false;
                for (const auto &margin: margins) {
                        T dist = {};
                        if (margin.intersects(ray, dist) && dist < margin_distance) {
                                intersects_any = true;
                                margin_distance = dist;
                        }
                }
                distance = margin_distance;
                return intersects_any && distance < 1000;
        }
        virtual Vec<3, T> getNorm(const Vec<3, T> &point) const {
                for (const auto &margin: margins) {
                        if (margin.contains(point)) {
                                return margin.getNorm(point);
                        }
                }
                return Vec((T)0, 1, 0);
        }
};

#endif

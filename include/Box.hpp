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
        std::pair<Vec<3, T>, Vec<3, T>> genOrthogonal(const Vec<3, T> &norm, const Vec<3, T> &corner_dir) {
                auto mp = norm * corner_dir;
                int imax = 0;
                T cmax = {};
                std::pair<Vec<3, T>, Vec<3, T>> norms;
                for (int i = 0; i < 3; ++i) {
                        if (std::abs(norm[i]) > std::abs(cmax)) {
                                cmax = norm[i];
                                imax = i;
                        }
                }
                switch (imax) {
                case 0:
                        norms = std::pair(Vec<3, T>(-norm[2], 0, norm[0]), Vec<3, T>(-norm[1], norm[0], 0));
                        break;
                case 1:
                        norms = std::pair(Vec<3, T>(0, -norm[2], norm[1]), Vec<3, T>(-norm[1], norm[0], 0));
                        break;
                case 2:
                        norms = std::pair(Vec<3, T>(-norm[2], 0, norm[0]), Vec<3, T>(0, -norm[2], norm[1]));
                        break;
                }
                norms.first = (norms.first - norm * (norms.first * norm)).normalize();
                norms.second = (norms.second - norm * (norms.second * norm) - norms.first * (norms.second * norms.first)).normalize();
                if ((norms.first * corner_dir) * mp < 0) norms.first = -norms.first;
                if ((norms.second * corner_dir) * mp < 0) norms.second = -norms.second;
                return norms;
        }
public:
        Box(const Vec<3, T> &center, const Vec<3, T> &corner_dir, const Vec<3, T> &norm, const Material<T> &material): material(material) {
                auto corner = center + corner_dir;
                auto opposite_corner = center - corner_dir;
                auto norms = genOrthogonal(norm, corner_dir);
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

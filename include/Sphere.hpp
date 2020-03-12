#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "Vec3.hpp"

template<typename T>
class Sphere {
private:
        Vec3<T> center;
        T radius;
public:
        using coord_type = T;
        Sphere(const Vec3<T> &center, T radius): center(center), radius(radius) {};
        bool intersects(const Vec3<T> &origin, const Vec3<T> &direction, T& distance) const {
                if (direction.length() == 0) {
                        return false;
                }
                auto v = center - origin;
                auto mult = v * direction;
                auto distance2 = v*v - mult*mult;
                if (distance2 > radius * radius) {
                        return false;
                }
                auto dist = std::sqrt(radius*radius - distance2);
                auto d1 = mult - dist;
                auto d2 = mult + dist;
                if (d1 < 0) {
                        distance = d2;
                } else {
                        distance = d1;
                }
                if (d1 < 0 && d2 < 0) {
                        return false;
                }
                return true;
        }
};

#endif

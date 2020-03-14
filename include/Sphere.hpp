#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "Material.hpp"
#include "Shape.hpp"
#include "Vec3.hpp"

template<typename T>
class Sphere: public Shape<T> {
private:
        Vec3<T> center;
        T radius;
        Material<T> material;
public:
        using coord_type = T;
        Sphere(const Vec3<T> &center, T radius, Material<T> &&material): center(center), radius(radius), material(material) {};
        virtual bool intersects(const Vec3<T> &origin, const Vec3<T> &direction, T& distance) const {
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

        virtual const Material<T>& getMaterial() const {
                return material;
        }

        virtual Vec3<T> getNorm(const Vec3<T> &point) const {
                return (point - center).normalize();
        }

        virtual ~Sphere() {}
};

#endif

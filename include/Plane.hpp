#ifndef _PLANE_HPP_
#define _PLANE_HPP_

#include "Material.hpp"
#include "Shape.hpp"
#include "Vec.hpp"

template<typename T>
class Plane: public Shape<T> {
private:
        Vec<3, T> norm;
        Vec<3, T> point;
        Material<T> material;
public:
        Plane(const Vec<3, T> &norm, const Vec<3, T> point, const Material<T> &material)
          : norm(norm), point(point), material(material){};
        virtual bool intersects(const Vec<3, T> &origin, const Vec<3, T> &direction, T& distance)  const {
                auto mp = direction * norm;
                auto distvec = origin - point;
                if (mp == 0) {
                        if (distvec * norm == 0) {
                                distance = 0;
                                return true;
                        } else {
                                return false;
                        }
                }
                auto t = (point - origin) * norm/mp;
                auto hitpoint = origin + direction * t;
                if (t >= 0) {
                        distance = (hitpoint - origin).length();
                        return true;
                }
                return false;
        }
        virtual const Material<T> &getMaterial() const {
                return material;
        }
        virtual Vec<3, T> getNorm(const Vec<3, T> &point) const {
                return norm;
        }
};

#endif

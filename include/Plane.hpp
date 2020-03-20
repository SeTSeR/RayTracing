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
        Plane(): norm(), point(), material() {};
        Plane(const Vec<3, T> &norm, const Vec<3, T> &point, const Material<T> &material)
          : norm(norm), point(point), material(material){};
        virtual bool intersects(const Ray<T> &ray, T& distance)  const {
                auto mp = ray.direction * norm;
                auto distvec = ray.origin - point;
                if (mp == 0) {
                        if (distvec * norm == 0) {
                                distance = 0;
                                return true;
                        } else {
                                return false;
                        }
                }
                auto t = -distvec * norm/mp;
                if (t >= 0) {
                        distance = t;
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
        bool contains(const Vec<3, T> &point) const {
                return (this->point - point) * norm == 0;
        }
};

#endif

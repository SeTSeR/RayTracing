#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include "Material.hpp"
#include "Vec3.hpp"

template<typename T>
class Shape {
public:
        virtual bool intersects(const Vec3<T> &origin, const Vec3<T> &direction, T& distance) const = 0;
        virtual const Material<T> &getMaterial() const = 0;
        virtual Vec3<T> getNorm(const Vec3<T> &point) const = 0;

        virtual ~Shape() {};
};

#endif

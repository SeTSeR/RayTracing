#ifndef _SHAPE_HPP_
#define _SHAPE_HPP_

#include "Material.hpp"
#include "Vec.hpp"

template<typename T>
class Shape {
public:
        virtual bool intersects(const Vec<3, T> &origin, const Vec<3, T> &direction, T& distance) const = 0;
        virtual const Material<T> &getMaterial() const = 0;
        virtual Vec<3, T> getNorm(const Vec<3, T> &point) const = 0;

        virtual ~Shape() {};
};

#endif

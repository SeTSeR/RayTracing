#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "Vec.hpp"

template<typename T>
struct Ray {
        Vec<3, T> origin;
        Vec<3, T> direction;
        Ray(): origin{}, direction{} {};
        Ray(const Vec<3, T> &origin, const Vec<3, T> &direction): origin(origin), direction(direction) {};
};

#endif

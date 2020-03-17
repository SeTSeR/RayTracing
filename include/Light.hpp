#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include "Vec.hpp"

template<typename T>
class Light {
private:
        Vec<3, T> position;
        T intensity;
public:
        Light(const Vec<3, T> &position, T intensity): position(position), intensity(intensity) {};
        const Vec<3, T> &getPosition() const {
                return position;
        }
        const T getIntensity() const {
                return intensity;
        }
};

#endif

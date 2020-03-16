#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include "Vec3.hpp"

template<typename T>
class Light {
private:
        Vec3<T> position;
        T intensity;
public:
        Light(const Vec3<T> &position, T intensity): position(position), intensity(intensity) {};
        const Vec3<T> &getPosition() const {
                return position;
        }
        const T getIntensity() const {
                return intensity;
        }
};

#endif

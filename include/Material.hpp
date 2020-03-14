#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "Vec3.hpp"

template<typename T>
class Material {
private:
        Vec3<T> diffuse_color;
public:
        Material(): diffuse_color() {};
        Material(Vec3<T> &&color): diffuse_color(color) {};
        const Vec3<T>& getColor() const {
                return diffuse_color;
        }
        static Material<T> ivory() {
                return Material(Vec3<T>(0.4, 0.4, 0.3));
        }
        static Material<T> red_rubber() {
                return Material(Vec3<T>(0.3, 0.1, 0.1));
        }
};

#endif

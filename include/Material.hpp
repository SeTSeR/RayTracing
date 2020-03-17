#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "Vec.hpp"

template<typename T>
class Material {
private:
        Vec<3, T> diffuse_color;
        Vec<2, T> albedo;
        T specular_exponent;
public:
        Material(): diffuse_color(), albedo(Vec<2, T>({(T)1., (T)0.})), specular_exponent() {};
        Material(Vec<3, T> &&color, Vec<2, T> &&albedo, T specular_exponent):
                diffuse_color(color), albedo(albedo), specular_exponent(specular_exponent) {};
        const Vec<3, T>& getDiffuseColor() const {
                return diffuse_color;
        }
        const Vec<2, T>& getAlbedo() const {
                return albedo;
        }
        const T getSpecularExponent() const {
                return specular_exponent;
        }
        static Material<T> ivory() {
                return Material(Vec<3, T>((T)0.4, (T)0.4, (T)0.3), Vec<2, T>((T)0.6, (T)0.3), 50.);
        }
        static Material<T> red_rubber() {
                return Material(Vec<3, T>((T)0.3, (T)0.1, (T)0.1), Vec<2, T>((T)0.9, (T)0.1), 10.);
        }
};

#endif

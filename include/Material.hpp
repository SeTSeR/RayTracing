#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "Vec.hpp"

template<typename T>
class Material {
private:
        Vec<3, T> diffuse_color;
        Vec<4, T> albedo;
        T specular_exponent;
        T refractive_index;
public:
        Material(): diffuse_color(), albedo(Vec((T)1., 0., 0., 0.)), specular_exponent() {};
        Material(Vec<3, T> &&color, Vec<4, T> &&albedo, T specular_exponent, T refractive_index):
                diffuse_color(color), albedo(albedo), specular_exponent(specular_exponent), refractive_index(refractive_index) {};
        const Vec<3, T>& getDiffuseColor() const {
                return diffuse_color;
        }
        const Vec<4, T>& getAlbedo() const {
                return albedo;
        }
        const T getSpecularExponent() const {
                return specular_exponent;
        }
        const T getRefractiveIndex() const {
                return refractive_index;
        }
        static Material<T> ivory() {
                return Material(Vec((T)0.4, 0.4, 0.3), Vec((T)0.6, 0.3, 0.1, 0.0), 50., 1.0);
        }
        static Material<T> red_rubber() {
                return Material(Vec((T)0.3, 0.1, 0.1), Vec((T)0.9, 0.1, 0.1, 0.0), 10., 1.0);
        }
        static Material<T> glass() {
                return Material(Vec((T)0.6, 0.7, 0.8), Vec((T)0.0, 0.5, 0.1, 0.8), 125., 1.5);
        }
        static Material<T> mirror() {
                return Material(Vec((T)1.0, 1.0, 1.0), Vec((T)0.0, 10.0, 0.8, 0.0), 1425., 1.0);
        }
        static Material<T> medium_gray() {
                return Material(Vec((T)0.454902f, 0.454902, 0.454902), Vec((T)0.8, 0.1, 0.1, 0.0), 12.8, 1.0);
        }
};

#endif

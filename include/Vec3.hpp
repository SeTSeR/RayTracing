#ifndef __VEC3_HPP__
#define __VEC3_HPP__

#include <cmath>

template<typename T>
class Vec3 {
private:
        T x;
        T y;
        T z;
public:
        using coord_type = T;
        Vec3(): x(T{}), y(T{}), z(T{}) {};
        Vec3(T x, T y, T z): x(x), y(y), z(z) {};
        T& operator[](int x) {
                switch (x) {
                case 0:
                        return this->x;
                case 1:
                        return this->y;
                case 2:
                        return this->z;
                default:
                        static_assert("Unreachable");
                }
        };
        Vec3& operator+=(const Vec3 &other) {
                x += other.x;
                y += other.y;
                z += other.z;
                return *this;
        };
        Vec3 operator+(const Vec3 &other) const {
                Vec3 result = *this;
                result += other;
                return result;
        };
        Vec3& operator-=(const Vec3 &other) {
                x -= other.x;
                y -= other.y;
                z -= other.z;
                return *this;
        };
        Vec3 operator-(const Vec3 &other) const {
                Vec3 result = *this;
                result -= other;
                return result;
        };
        Vec3 operator*=(T coef) {
                x *= coef;
                y *= coef;
                z *= coef;
                return *this;
        };
        Vec3 operator*(T coef) const {
                Vec3 result = *this;
                result *= coef;
                return result;
        };
        T operator*(const Vec3 &other) const {
                return x * other.x + y * other.y + z * other.z;
        };
        T length() const {
                return std::sqrt(x*x + y*y + z*z);
        }
        Vec3& normalize() {
                auto len = std::sqrt(x*x + y*y + z*z);
                x /= len;
                y /= len;
                z /= len;
                return *this;
        }
};

#endif

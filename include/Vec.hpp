#ifndef __VEC3_HPP__
#define __VEC3_HPP__

#include <array>
#include <cmath>

template<size_t size, typename T>
class Vec {
private:
        std::array<T, size> arr;
public:
        using coord_type = T;
        Vec(): arr({}) {};
        template <typename ...Args,
                  typename = std::enable_if_t<
                      sizeof...(Args) == size &&
                      (std::is_same_v<T, std::decay_t<Args>> && ...)>>
        Vec(Args &&...args) : arr{std::forward<Args>(args)...} {};
        T& operator[](int x) {
                return arr[x];
        };
        const T& operator[](int x) const {
                return arr[x];
        };
        Vec& operator+=(const Vec &other) {
                for (int i = 0; i < size; ++i) {
                        arr[i] += other.arr[i];
                }
                return *this;
        };
        Vec operator+(const Vec &other) const {
                Vec result = *this;
                result += other;
                return result;
        };
        Vec& operator-=(const Vec &other) {
                for (int i = 0; i < size; ++i) {
                        arr[i] -= other.arr[i];
                }
                return *this;
        };
        Vec operator-(const Vec &other) const {
                Vec result = *this;
                result -= other;
                return result;
        };
        Vec operator*=(T coef) {
                for (int i = 0; i < size; ++i) {
                        arr[i] *= coef;
                }
                return *this;
        };
        Vec operator*(T coef) const {
                Vec result = *this;
                result *= coef;
                return result;
        };
        T operator*(const Vec &other) const {
                T sum = {};
                for (int i = 0; i < size; ++i) {
                        sum += arr[i] * other.arr[i];
                }
                return sum;
        };
        T length() const {
                return std::sqrt(*this * *this);
        }
        Vec& normalize() {
                auto len = length();
                *this *= (T)1 / len;
                return *this;
        }
        Vec reflect(const Vec &norm) {
                return *this - norm * 2. * (norm * *this);
        }
};

#endif

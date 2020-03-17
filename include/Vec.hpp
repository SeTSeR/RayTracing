#ifndef __VEC3_HPP__
#define __VEC3_HPP__

#include <array>
#include <cmath>
#include <optional>

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
                      (std::is_convertible_v<T, std::decay_t<Args>> && ...)>>
        Vec(Args &&...args) : arr{static_cast<T>(std::forward<Args>(args))...} {};
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
        Vec operator-() const {
                return Vec() - *this;
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
        Vec reflect(const Vec &norm) const {
                return *this - norm * 2. * (norm * *this);
        }
        std::optional<Vec> refract(const Vec &norm, T n1, T n2 = 1.0) const {
                T c  = - *this * norm;
                if (c < 0) {
                        c = -c;
                        T tmp = n1;
                        n1 = n2;
                        n2 = n1;
                }
                T r = n1 / n2;
                T k = 1 - r * r * (1 - c * c);
                if (k < 0) {
                        return {};
                }
                return *this * r + norm * (r * c - std::sqrt(k));
        }
};

template <typename Arg, typename ... Rest>
Vec(Arg, Rest...) -> Vec<sizeof...(Rest) + 1, Arg>;

#endif

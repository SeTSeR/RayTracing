#ifndef __VEC3_HPP__
#define __VEC3_HPP__

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <ostream>

template<size_t size, typename T>
class Vec {
private:
        std::array<T, size> arr;
        Vec(const std::array<T, size> &arr): arr(arr) {};
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
        Vec operator/(T coef) const {
                return *this * (1. / coef);
        };
        T operator*(const Vec &other) const {
                T sum = {};
                for (int i = 0; i < size; ++i) {
                        sum += arr[i] * other.arr[i];
                }
                return sum;
        };
        Vec mult(const Vec &other) const {
                Vec result = *this;
                for (int i = 0; i < size; ++i) {
                        result.arr[i] *= other.arr[i];
                }
                return result;
        };
        Vec sqrt() const {
                std::array<T, size> new_arr;
                std::transform(arr.begin(), arr.end(), new_arr.begin(), [](T x) -> T { return std::sqrt(x); });
                return Vec(new_arr);
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
        static Vec unitVecInHemisphere(const Vec &norm, T r1, T r2) {
                static_assert(size == 3);
                T phi = 2 * M_PI * r1;
                T h = 2 * r2 - 1;
                return Vec<3, float>(std::sin(phi) * std::sqrt(1 - h * h),
                                     std::cos(phi) * std::sqrt(1 - h * h), h).normalize();
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
                        return std::nullopt;
                }
                return *this * r + norm * (r * c - std::sqrt(k));
        }
        friend std::ostream& operator<<(std::ostream& os, const Vec& v) {
                for (const auto &elem: v.arr) {
                        os << elem << " ";
                }
                return os;
        }
        friend Vec operator*(T r, const Vec& v) {
                return v * r;
        }
};

template <typename Arg, typename ... Rest>
Vec(Arg, Rest...) -> Vec<sizeof...(Rest) + 1, Arg>;

#endif

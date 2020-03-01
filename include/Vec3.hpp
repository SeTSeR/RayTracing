#ifndef __VEC3_HPP__
#define __VEC3_HPP__

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
};

#endif

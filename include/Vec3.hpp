template<typename T>
class Vec3 {
private:
        T x;
        T y;
        T z;
public:
        using coord_type = T;
        Vec3(T x, T y, T z): x(x), y(y), z(z) {};
        ~Vec3();
};

#include <Image.hpp>
#include <Render.hpp>
#include <Sphere.hpp>
#include <Vec3.hpp>

#include <cmath>
#include <vector>

Vec3<float> cast_ray(const Vec3<float> &origin, const Vec3<float> &direction, const Sphere<float> &sphere) {
        if (!sphere.intersects(origin, direction)) {
                return Vec3(0.2f, 0.7f, 0.8f);
        }
        return Vec3(0.4f, 0.4f, 0.3f);
}

void render(int width, int height, const std::filesystem::path &path) {
        Image framebuffer(width, height);

        float fov = M_PI/2;
        Sphere sphere(Vec3<float>(-3.f, 0.f, -16.f), 2.f);

        for (size_t j = 0; j < height; ++j) {
                for (size_t i = 0; i < width; ++i) {
                        float x = (2*(i + 0.5)/(float)width - 1)*tan(fov/2.)*width/(float)height;
                        float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
                        Vec3<float> dir = Vec3<float>(x, y, -1).normalize();
                        framebuffer[j][i] = cast_ray(Vec3<float>(0, 0, 0), dir, sphere);
                }
        }

        framebuffer.write_png(path);
}

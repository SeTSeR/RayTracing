#include "Image.hpp"
#include "Material.hpp"
#include "Plane.hpp"
#include "Render.hpp"
#include "Sphere.hpp"
#include "Vec.hpp"

#include <cmath>
#include <memory>
#include <vector>

#include <omp.h>

Render::Render() {
        Scene<float> scene;
        scene.addShape(std::make_unique<Sphere<float>>(Vec(-3.f, 0, -16), 2.f, Material<float>::ivory()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec(-1.f, -1.5, -12), 2.f, Material<float>::glass()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec(1.5f, -0.5, -18), 3.f, Material<float>::red_rubber()));
        scene.addShape(std::make_unique<Sphere<float>>(Vec(7.f, 5, -18), 4.f, Material<float>::mirror()));
        scene.addShape(std::make_unique<Plane<float>>(Vec(0.f, 1, 0), Vec(-1.f, -4, -12), Material<float>::ivory()));
        scene.addLight(Light(Vec(-20.f, 20, 20), 1.5f));
        scene.addLight(Light(Vec(30.f, 50, -25), 1.8f));
        scene.addLight(Light(Vec(30.f, 20, 30), 1.7f));
        scenes.push_back(std::move(scene));

        Scene<float> scene2;
        scene2.addShape(std::make_unique<Plane<float>>(Vec(0.f, 1, 0), Vec(0.f, -3.5, 0), Material<float>::medium_gray()));
        scene2.addShape(std::make_unique<Plane<float>>(Vec(1.f, 0, 0), Vec(-8.f, 0, 0), Material<float>::medium_gray()));
        scene2.addShape(std::make_unique<Plane<float>>(Vec(-1.f, 0, 0), Vec(8.f, 0, 0), Material<float>::medium_gray()));
        scene2.addShape(std::make_unique<Plane<float>>(Vec(0.f, -1, 0), Vec(0.f, 5, 0), Material<float>::medium_gray()));
        scene2.addShape(std::make_unique<Plane<float>>(Vec(0.f, 0, 1), Vec(0.f, 0.f, -9), Material<float>::medium_gray()));
        scene2.addShape(std::make_unique<Sphere<float>>(Vec(-3.f, 0, -3), 1.f, Material<float>::ivory()));
        scene2.addShape(std::make_unique<Sphere<float>>(Vec(3.f, 3, -6), 1.f, Material<float>::red_rubber()));
        //scene2.addShape(std::make_unique<Plane<float>>(Vec(1.f, 0, -1).normalize(), Vec(0.f, 0, -4), Material<float>::glass()));
        scene2.addLight(Light(Vec(0.f, 2.5, 10), 2.f));
        scene2.addLight(Light(Vec(4.f, 0, -7), 1.7f));
        scenes.push_back(std::move(scene2));
}

Vec<3, float> Render::castRay(const Ray<float> &ray, const Scene<float> &scene, const Vec<3, float> &default_color, int depth) {
        Vec<3, float> point, norm;
        Material<float> material;
        if (depth > MAX_DEPTH || !scene.intersects(ray, point, norm, material)) {
                return default_color;
        }
        Vec reflect_direction = ray.direction.reflect(norm).normalize();
        Vec reflect_origin = reflect_direction * norm < 0 ? point - norm * 1e-3 : point + norm * 1e-3;
        Vec reflect_color = castRay(Ray(reflect_origin, reflect_direction), scene, default_color, depth + 1);

        std::optional refract_direction = ray.direction.refract(norm, material.getRefractiveIndex())->normalize();
        if (refract_direction) {
                Vec refract_origin = *refract_direction * norm < 0 ? point - norm * 1e-3 : point + norm * 1e-3;
                Vec refract_color = castRay(Ray(refract_origin, *refract_direction), scene, default_color, depth + 1);
                return scene.light_color(point, ray.direction, norm, material, reflect_color, refract_color);
        } else {
                return scene.light_color(point, ray.direction, norm, material, reflect_color, Vec(0.f, 0, 0));
        }
}

Vec<3, float> Render::tracePath(const Ray<float> &ray, const Scene<float> &scene, const Vec<3, float> &default_color, int depth) {
        if (depth == MAX_DEPTH) {
                return default_color;
        }
        Vec<3, float> point, norm;
        Material<float> material;
        if (!scene.intersects(ray, point, norm, material)) {
                return default_color;
        }

        Vec<3, float> directLighting = {};
        Vec<3, float> indirectLighting;
        float Sigma = material.getAlbedo() * Vec(1.f, 1, 1, 1);
        float xi = dis(gen);
        float r1 = dis(gen);
        float r2 = dis(gen);
        Vec<3, float> BRDF;
        float PDF;
        Ray<float> newRay;
        float cos;
        if (xi <= material.getAlbedo()[0] / Sigma) {
                for (const auto &light: scene.getLights()) {
                        Vec light_direction = (light.getPosition() - point).normalize();
                        auto light_distance = (light.getPosition() - point).length();
                        Vec shadow_orig = light_direction * norm < 0 ? point - norm * 1e-3 : point + norm * 1e-3;
                        Material<float> mat;
                        Vec<3, float> hit, n;
                        if (!scene.intersects(Ray(shadow_orig, light_direction), hit, n, mat) || (hit - shadow_orig).length() >= light_distance) {
                                directLighting += light.getIntensity() * std::max(0.f, light_direction * norm) * material.getDiffuseColor();
                        }
                }
                
                newRay = Ray<float>(point, Vec<3, float>::cosineVecInHemisphere(norm, r1, r2));
                cos = newRay.direction * norm;
                BRDF = (1 / M_PI) * Vec(1.f, 1, 1);
                PDF = cos / M_PI;
        } else if (xi <= (material.getAlbedo()[0] + material.getAlbedo()[1] + material.getAlbedo()[2]) / Sigma) {
                Vec reflect_direction = ray.direction.reflect(norm).normalize();
                Vec new_direction = Vec<3, float>::fixedPhongVec(r1, r2, material.getSpecularExponent());
                newRay = Ray<float>(point, new_direction);
                cos = new_direction * norm;
                BRDF = (material.getSpecularExponent() + 2) / (2 * M_PI) * std::pow(cos, material.getSpecularExponent()) * Vec(1.f, 1, 1);
                PDF = (material.getSpecularExponent() + 1) / (2 * M_PI) * std::pow(cos, material.getSpecularExponent());
        } else {
                std::optional refract_direction = ray.direction.refract(norm, material.getRefractiveIndex())->normalize();
                if (refract_direction) {
                        newRay = Ray<float>(point, Vec<3, float>::cosineVecInHemisphere(*refract_direction, r1, r2));
                        cos = newRay.direction * norm;
                        BRDF = (1 / M_PI) * Vec(1.f, 1, 1);
                        PDF = cos / M_PI;
                } else return default_color;
        }
        indirectLighting = ((cos / PDF) * BRDF).mult(tracePath(newRay, scene, default_color, depth + 1));
        return (directLighting / M_PI + indirectLighting);
}

void Render::renderImage(const Config &config) {
        Image framebuffer(config.width, config.height);
        Image background(config.width, config.height);

        float fov = M_PI/2;

        if (config.scene_num > scenes.size()) return;

        if (config.scene_num == 1) {
                render_mode = RAY_TRACER;
        } else {
                render_mode = PATH_TRACER;
        }

        if (config.background_path) {
                background.read_image(*config.background_path);
        } else {
                for (size_t j = 0; j < config.height; ++j) {
                        for (size_t i = 0; i < config.width; ++i) {
                                background[j][i] = Vec(0.2f, 0.7, 0.8);
                        }
                }
        }

        if (config.threads_num) {
                omp_set_num_threads(*config.threads_num);
        }

#pragma omp parallel for shedule(dynamic);
        for (size_t j = 0; j < config.height; ++j) {
                for (size_t i = 0; i < config.width; ++i) {
                        Vec<3, float> cell_color = {};
                        if (render_mode == RAY_TRACER) {
                                for (size_t k = 0; k < SAMPLES_COUNT; ++k) {
                                        for (size_t l = 0; l < SAMPLES_COUNT; ++l) {
                                                float x = (2*(i*SAMPLES_COUNT + k + 0.5)/(float)(config.width*SAMPLES_COUNT) - 1)*tan(fov/2.)*config.width/(float)config.height;
                                                float y = -(2*(j*SAMPLES_COUNT + l + 0.5)/(float)(config.height*SAMPLES_COUNT) - 1)*tan(fov/2.);
                                                Vec dir = Vec(x, y, -1).normalize();
                                                cell_color += castRay(Ray(Vec(0.f, 0, 0), dir), scenes[config.scene_num - 1], background[j][i]);
                                        }
                                }
                                cell_color *= (1.f / (SAMPLES_COUNT * SAMPLES_COUNT));
                        } else {
                                float x = (2*(i + 0.5)/(float)config.width - 1)*tan(fov/2.)*config.width/(float)config.height;
                                float y = -(2*(j + 0.5)/(float)config.height - 1)*tan(fov/2.);
                                Vec<3, float> dir = Vec(x, y, -1).normalize();
                                const float desired_error = 0.5;
                                Vec<3, float> err(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
                                Vec<3, float> sum_color = {};
                                Vec<3, float> sum_sq_color = {};
                                int i = 0;
                                while (err.length() >= desired_error) {
                                        cell_color = ((float)i / (i + 1)) * cell_color + (1. / (i + 1)) * tracePath(Ray(Vec(0.f, 0, 0), dir), scenes[config.scene_num - 1], background[j][i]);
                                        if (i > 10) {
                                                sum_color += cell_color;
                                                sum_sq_color += cell_color.mult(cell_color);
                                                auto avg = sum_color / (i + 1.f);
                                                err = (sum_sq_color / (i + 1.f) -  avg.mult(avg)).sqrt();
                                        }
                                        ++i;
                                }
                        }
                        framebuffer[j][i] = cell_color;
                        float max = std::max(framebuffer[j][i][0], std::max(framebuffer[j][i][1], framebuffer[j][i][2]));
                        if (max > 1) framebuffer[j][i] *= (1./max);
                }
        }

        framebuffer.write_png(config.output_path);
}

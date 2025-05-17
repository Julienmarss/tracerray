/*
** EPITECH PROJECT, 2025
** B-OOP-400-LIL-4-1-raytracer-yanis.asselman
** File description:
** plugin_cone - Version simple sans base
*/

#include "../include/plugins/plugin_cone.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include <libconfig.h++>
#include <iostream>

RayTracer::Cone::Cone(const Vector3D& apex, double height, double radius, const Material& mat)
    : _apex(apex), _height(height), _radius(radius), _material(mat), _inverted(false) {}

RayTracer::Cone::Cone(const Vector3D& apex, double height, double radius, const Material& mat, bool inverted)
    : _apex(apex), _height(height), _radius(radius), _material(mat), _inverted(inverted) {}

std::optional<RayTracer::HitInfo> RayTracer::Cone::intersect(const Ray& ray, double minDist, double maxDist) const
{
    double dir_multiplier = _inverted ? -1.0 : 1.0;
    RayTracer::Vector3D tip = _apex + RayTracer::Vector3D(0, 0, _height * dir_multiplier);
    RayTracer::Vector3D v = ray.getDirection();
    RayTracer::Vector3D u = ray.getOrigin() - tip;
    double k = _radius / _height;
    double k2 = k * k;
    double a = v.getX() * v.getX() + v.getY() * v.getY() - k2 * v.getZ() * v.getZ();
    double b = 2.0 * (u.getX() * v.getX() + u.getY() * v.getY() - k2 * u.getZ() * v.getZ());
    double c = u.getX() * u.getX() + u.getY() * u.getY() - k2 * u.getZ() * u.getZ();
    double discriminant = b * b - 4.0 * a * c;
    
    if (discriminant < 0.0) {
        return std::nullopt;
    }

    double sqrt_discriminant = std::sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);
    
    for (double t : {t1, t2}) {
        if (t >= minDist && t <= maxDist) {
            RayTracer::Vector3D point = ray.getOrigin() + v * t;
            double z_diff = (point.getZ() - _apex.getZ()) * dir_multiplier;
            
            if (z_diff >= 0.0 && z_diff <= _height) {
                double expected_radius = _radius * (1.0 - z_diff / _height);
                double actual_radius_sq = (point.getX() - _apex.getX()) * (point.getX() - _apex.getX()) + 
                                         (point.getY() - _apex.getY()) * (point.getY() - _apex.getY());
                
                if (actual_radius_sq <= expected_radius * expected_radius + 1e-6) {
                    RayTracer::Vector3D to_point = point - _apex;
                    RayTracer::Vector3D radial(to_point.getX(), to_point.getY(), 0);
                    RayTracer::Vector3D normal;
                    if (radial.length() > 1e-6) {
                        radial = radial.normalize();
                        double cone_angle = std::atan(_radius / _height);
                        RayTracer::Vector3D axial(0, 0, dir_multiplier);
                        normal = radial * std::cos(cone_angle) + axial * std::sin(cone_angle);
                        normal = normal.normalize();
                    } else {
                        normal = RayTracer::Vector3D(0, 0, dir_multiplier);
                    }
                    return RayTracer::HitInfo{t, point, normal, _material};
                }
            }
        }
    }
    return std::nullopt;
}

const RayTracer::Material& RayTracer::Cone::getMaterial() const
{
    return _material;
}

void RayTracer::Cone::setMaterial(const RayTracer::Material& material) {
    _material = material;
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::Cone::clone() const
{
    return std::make_unique<Cone>(_apex, _height, _radius, _material, _inverted);
}

extern "C" RayTracer::IPrimitive* create(const libconfig::Setting& config)
{
    RayTracer::Vector3D position(0.0, 0.0, 0.0);
    double height = 50.0;
    double radius = 20.0;
    bool inverted = false;
    RayTracer::Color color(static_cast<uint8_t>(128), static_cast<uint8_t>(0), static_cast<uint8_t>(255));
    
    if (config.exists("position")) {
        const auto& pos = config["position"];
        position = RayTracer::Vector3D(pos["x"], pos["y"], pos["z"]);
    }
    if (config.exists("color")) {
        const auto& col = config["color"];
        color = RayTracer::Color(
            static_cast<uint8_t>((int)col["r"]),
            static_cast<uint8_t>((int)col["g"]),
            static_cast<uint8_t>((int)col["b"])
        );
    }
    if (config.exists("height"))
        height = config["height"];
    if (config.exists("radius"))
        radius = config["radius"];
    if (config.exists("inverted"))
        inverted = config["inverted"];
    RayTracer::Material mat(color, 0.5, 0.5);
    return new RayTracer::Cone(position, height, radius, mat, inverted);
}

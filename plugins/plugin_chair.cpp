/*
** EPITECH PROJECT, 2025
** B-OOP-400-LIL-4-1-raytracer-yanis.asselman
** File description:
** plugin_chair - Version sans namespace
*/

#include "../include/plugins/plugin_chair.hpp"
#include <cmath>
#include <iostream>

RayTracer::Box::Box(const Vector3D& min, const Vector3D& max, const Material& mat)
    : _min(min), _max(max), _material(mat) {}

std::optional<RayTracer::HitInfo> RayTracer::Box::intersect(const Ray& ray, double minDist, double maxDist) const
{
    RayTracer::Vector3D invDir(1.0 / ray.getDirection().getX(),
                              1.0 / ray.getDirection().getY(),
                              1.0 / ray.getDirection().getZ());
    RayTracer::Vector3D orig = ray.getOrigin();

    double tmin = (_min.getX() - orig.getX()) * invDir.getX();
    double tmax = (_max.getX() - orig.getX()) * invDir.getX();
    if (tmin > tmax) std::swap(tmin, tmax);

    double tymin = (_min.getY() - orig.getY()) * invDir.getY();
    double tymax = (_max.getY() - orig.getY()) * invDir.getY();
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return std::nullopt;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    double tzmin = (_min.getZ() - orig.getZ()) * invDir.getZ();
    double tzmax = (_max.getZ() - orig.getZ()) * invDir.getZ();
    if (tzmin > tzmax) std::swap(tzmin, tzmax);
    if ((tmin > tzmax) || (tzmin > tmax))
        return std::nullopt;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    double t = tmin;
    if (t < minDist || t > maxDist)
        return std::nullopt;

    RayTracer::Vector3D point = orig + ray.getDirection() * t;
    RayTracer::Vector3D normal(0, 0, 1);
    RayTracer::Vector3D center = (_min + _max) * 0.5;
    RayTracer::Vector3D offset = point - center;
    RayTracer::Vector3D size = _max - _min;
    
    if (std::abs(offset.getX()) / size.getX() > std::abs(offset.getY()) / size.getY() &&
        std::abs(offset.getX()) / size.getX() > std::abs(offset.getZ()) / size.getZ()) {
        normal = RayTracer::Vector3D(offset.getX() > 0 ? 1 : -1, 0, 0);
    } else if (std::abs(offset.getY()) / size.getY() > std::abs(offset.getZ()) / size.getZ()) {
        normal = RayTracer::Vector3D(0, offset.getY() > 0 ? 1 : -1, 0);
    } else {
        normal = RayTracer::Vector3D(0, 0, offset.getZ() > 0 ? 1 : -1);
    }
    return RayTracer::HitInfo{t, point, normal, _material};
}

const RayTracer::Material& RayTracer::Box::getMaterial() const
{
    return _material;
}

void RayTracer::Box::setMaterial(const RayTracer::Material& material)
{
    _material = material;
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::Box::clone() const
{
    return std::make_unique<Box>(*this);
}

RayTracer::Chair::Chair()
{
    RayTracer::Material wood(RayTracer::Color(static_cast<uint8_t>(139), static_cast<uint8_t>(69), static_cast<uint8_t>(19)), 0.3, 0.6);
    addBox(RayTracer::Vector3D(-20, -20, 0), RayTracer::Vector3D(20, 20, 5), wood);
    addBox(RayTracer::Vector3D(-20, 17, 5), RayTracer::Vector3D(20, 22, 40), wood);
    addBox(RayTracer::Vector3D(-18, -18, -30), RayTracer::Vector3D(-13, -13, 0), wood);
    addBox(RayTracer::Vector3D(13, -18, -30), RayTracer::Vector3D(18, -13, 0), wood);
    addBox(RayTracer::Vector3D(-18, 13, -30), RayTracer::Vector3D(-13, 18, 0), wood);
    addBox(RayTracer::Vector3D(13, 13, -30), RayTracer::Vector3D(18, 18, 0), wood);
}

RayTracer::Chair::Chair(const Chair& other)
{
    for (const auto& part : other.parts) {
        parts.push_back(part->clone());
    }
}

std::optional<RayTracer::HitInfo> RayTracer::Chair::intersect(const Ray& ray, double minDist, double maxDist) const
{
    std::optional<RayTracer::HitInfo> closest;
    double closestDist = maxDist;
    
    for (const auto& part : parts) {
        auto hit = part->intersect(ray, minDist, closestDist);
        if (hit) {
            closestDist = hit->distance;
            closest = hit;
        }
    }
    return closest;
}

const RayTracer::Material& RayTracer::Chair::getMaterial() const
{
    if (parts.empty()) {
        static RayTracer::Material defaultMat;
        return defaultMat;
    }
    return parts.front()->getMaterial();
}

void RayTracer::Chair::setMaterial(const RayTracer::Material& material)
{
    for (auto& part : parts) {
        part->setMaterial(material);
    }
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::Chair::clone() const
{
    return std::make_unique<Chair>(*this);
}

void RayTracer::Chair::addBox(const RayTracer::Vector3D& min, const RayTracer::Vector3D& max, const RayTracer::Material& mat)
{
    parts.push_back(std::make_unique<RayTracer::Box>(min, max, mat));
}

extern "C" RayTracer::IPrimitive* create()
{
    return new RayTracer::Chair();
}
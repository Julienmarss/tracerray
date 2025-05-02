#include "../../include/Core/Ray.hpp"

RayTracer::Ray::Ray() : origin(0, 0, 0), direction(0, 0, 1) {}

RayTracer::Ray::Ray(const Vector3D &origin, const Vector3D &direction)
    : origin(origin), direction(direction.normalize()) {}

const RayTracer::Vector3D& RayTracer::Ray::getOrigin() const {
    return origin;
}

const RayTracer::Vector3D& RayTracer::Ray::getDirection() const {
    return direction;
}

RayTracer::Vector3D RayTracer::Ray::pointAt(double t) const {
    return origin + direction * t;
}
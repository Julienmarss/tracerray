#include "../../include/Primitives/Plane.hpp"
#include <cmath>
#include <stdexcept>

RayTracer::Plane::Plane() : normal(0, 0, 1), distance(0), material() {}

RayTracer::Plane::Plane(const Vector3D &normal, double distance, const Material &material)
    : normal(normal.normalize()), distance(distance), material(material) {}

RayTracer::Plane::Plane(char axis, double position, const Material &material)
    : normal(axisToNormal(axis)), distance(position), material(material) {}

RayTracer::Vector3D RayTracer::Plane::axisToNormal(char axis) {
    switch (axis) {
        case 'X':
        case 'x':
            return Vector3D(1, 0, 0);
        case 'Y':
        case 'y':
            return Vector3D(0, 1, 0);
        case 'Z':
        case 'z':
            return Vector3D(0, 0, 1);
        default:
            throw std::invalid_argument("Invalid axis. Must be 'X', 'Y', or 'Z'");
    }
}

std::optional<RayTracer::HitInfo> RayTracer::Plane::intersect(const Ray &ray, double minDist, double maxDist) const {
    double denom = normal.dot(ray.getDirection());
    const double EPSILON = 1e-8;
    if (std::fabs(denom) < EPSILON) {
        return std::nullopt;
    }
    
    double t = (distance - normal.dot(ray.getOrigin())) / denom;
    
    if (t < minDist || t > maxDist) {
        return std::nullopt;
    }
    
    RayTracer::HitInfo hitInfo;
    hitInfo.distance = t;
    hitInfo.point = ray.pointAt(t);
    hitInfo.normal = (denom > 0) ? -normal : normal;
    hitInfo.material = material;
    return hitInfo;
}

const RayTracer::Material& RayTracer::Plane::getMaterial() const {
    return material;
}

void RayTracer::Plane::setMaterial(const Material &material) {
    this->material = material;
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::Plane::clone() const {
    return std::make_unique<Plane>(*this);
}

const RayTracer::Vector3D& RayTracer::Plane::getNormal() const {
    return normal;
}

double RayTracer::Plane::getDistance() const {
    return distance;
}

void RayTracer::Plane::setNormal(const Vector3D &normal) {
    this->normal = normal.normalize();
}

void RayTracer::Plane::setDistance(double distance) {
    this->distance = distance;
}
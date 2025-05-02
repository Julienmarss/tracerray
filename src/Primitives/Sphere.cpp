#include "../../include/Primitives/Sphere.hpp"
#include <cmath>

RayTracer::Sphere::Sphere() : center(0, 0, 0), radius(1.0), material() {}

RayTracer::Sphere::Sphere(const Vector3D &center, double radius, const Material &material)
    : center(center), radius(radius > 0 ? radius : 0), material(material) {}

std::optional<RayTracer::HitInfo> RayTracer::Sphere::intersect(const Ray &ray, double minDist, double maxDist) const {
    Vector3D oc = ray.getOrigin() - center;
    
    double a = 1.0;
    double half_b = oc.dot(ray.getDirection());
    double c = oc.lengthSquared() - radius * radius;
    double discriminant = half_b * half_b - a * c;
    
    if (discriminant < 0) {
        return std::nullopt;
    }
    
    double sqrtd = std::sqrt(discriminant);
    
    double root = -half_b - sqrtd;
    if (root < minDist || root > maxDist) {
        root = -half_b + sqrtd;
        if (root < minDist || root > maxDist) {
            return std::nullopt;
        }
    }
    
    RayTracer::HitInfo hitInfo;
    hitInfo.distance = root;
    hitInfo.point = ray.pointAt(root);
    hitInfo.normal = (hitInfo.point - center) / radius;
    hitInfo.material = material;
    return hitInfo;
}

const RayTracer::Material& RayTracer::Sphere::getMaterial() const {
    return material;
}

void RayTracer::Sphere::setMaterial(const Material &material) {
    this->material = material;
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::Sphere::clone() const {
    return std::make_unique<Sphere>(*this);
}

const RayTracer::Vector3D& RayTracer::Sphere::getCenter() const {
    return center;
}

double RayTracer::Sphere::getRadius() const {
    return radius;
}

void RayTracer::Sphere::setCenter(const Vector3D &center) {
    this->center = center;
}

void RayTracer::Sphere::setRadius(double radius) {
    this->radius = (radius > 0) ? radius : 0;
}
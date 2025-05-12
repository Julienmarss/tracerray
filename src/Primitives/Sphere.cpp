#include "../../include/Primitives/Sphere.hpp"
#include <cmath>

RayTracer::Sphere::Sphere() : center(0, 0, 0), radius(1.0), material() {}

RayTracer::Sphere::Sphere(const Vector3D &center, double radius, const Material &material)
    : center(center), radius(radius > 0 ? radius : 0), material(material) {}

std::optional<RayTracer::HitInfo> RayTracer::Sphere::intersect(const Ray &ray, double minDist, double maxDist) const {
    Vector3D oc = ray.getOrigin() - center;
    
    double a = ray.getDirection().dot(ray.getDirection());
    double b = 2.0 * oc.dot(ray.getDirection());
    double c = oc.dot(oc) - radius*radius;
    
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return std::nullopt;
    }
    
    double t = (-b - sqrt(discriminant)) / (2.0*a);
    if (t < minDist || t > maxDist) {
        t = (-b + sqrt(discriminant)) / (2.0*a);
        if (t < minDist || t > maxDist) {
            return std::nullopt;
        }
    }
    
    RayTracer::HitInfo hitInfo;
    hitInfo.distance = t;
    hitInfo.point = ray.pointAt(t);
    Vector3D outNormal = (hitInfo.point - center) / radius;
    hitInfo.normal = outNormal.normalize();
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
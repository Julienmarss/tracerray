#include "../../include/Primitives/Cylinder.hpp"
#include <cmath>
#include <algorithm>

RayTracer::Cylinder::Cylinder(const Vector3D& position, double radius, double height, const Material& material)
    : _position(position), _radius(radius), _height(height), _material(material) {}

RayTracer::Vector3D RayTracer::Cylinder::getPosition() const
{ 
    return _position; 
}

double RayTracer::Cylinder::getRadius() const
{ 
    return _radius; 
}

double RayTracer::Cylinder::getHeight() const
{ 
    return _height; 
}

const RayTracer::Material& RayTracer::Cylinder::getMaterial() const
{ 
    return _material; 
}

std::optional<RayTracer::HitInfo> RayTracer::Cylinder::intersect(const Ray &ray, double minDist, double maxDist) const
{
    std::optional<RayTracer::HitInfo> closest_hit;
    double closest_distance = maxDist;
    double ox = ray.getOrigin().getX() - _position.getX();
    double oz = ray.getOrigin().getZ() - _position.getZ();
    double dx = ray.getDirection().getX();
    double dz = ray.getDirection().getZ();
    double a = dx * dx + dz * dz;
    double b = 2.0 * (ox * dx + oz * dz);
    double c = ox * ox + oz * oz - _radius * _radius;
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant >= 0.0 && a > 1e-8) {
        double sqrt_disc = std::sqrt(discriminant);
        double t1 = (-b - sqrt_disc) / (2.0 * a);
        double t2 = (-b + sqrt_disc) / (2.0 * a);

        for (double t : {t1, t2}) {
            if (t >= minDist && t < closest_distance) {
                RayTracer::Vector3D point = ray.getOrigin() + ray.getDirection() * t;
                double y = point.getY();
                if (y >= _position.getY() && y <= _position.getY() + _height) {
                    RayTracer::Vector3D radial(
                        point.getX() - _position.getX(),
                        0.0,
                        point.getZ() - _position.getZ()
                    );
                    RayTracer::Vector3D normal = radial.normalize();
                    closest_distance = t;
                    closest_hit = RayTracer::HitInfo{t, point, normal, _material};
                }
            }
        }
    }
    
    if (std::abs(ray.getDirection().getY()) > 1e-8) {
        double t = (_position.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
        
        if (t >= minDist && t < closest_distance) {
            RayTracer::Vector3D point = ray.getOrigin() + ray.getDirection() * t;
            double dx = point.getX() - _position.getX();
            double dz = point.getZ() - _position.getZ();
            double dist_sq = dx * dx + dz * dz;
            if (dist_sq <= _radius * _radius) {
                RayTracer::Vector3D normal(0, -1, 0);
                closest_distance = t;
                closest_hit = RayTracer::HitInfo{t, point, normal, _material};
            }
        }
    }
    
    if (std::abs(ray.getDirection().getY()) > 1e-8) {
        double t = (_position.getY() + _height - ray.getOrigin().getY()) / ray.getDirection().getY();
        if (t >= minDist && t < closest_distance) {
            RayTracer::Vector3D point = ray.getOrigin() + ray.getDirection() * t;
            double dx = point.getX() - _position.getX();
            double dz = point.getZ() - _position.getZ();
            double dist_sq = dx * dx + dz * dz;

            if (dist_sq <= _radius * _radius) {
                RayTracer::Vector3D normal(0, 1, 0);
                closest_distance = t;
                closest_hit = RayTracer::HitInfo{t, point, normal, _material};
            }
        }
    }
    return closest_hit;
}

void RayTracer::Cylinder::setMaterial(const Material &material)
{
    _material = material;
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::Cylinder::clone() const
{
    return std::make_unique<Cylinder>(_position, _radius, _height, _material);
}
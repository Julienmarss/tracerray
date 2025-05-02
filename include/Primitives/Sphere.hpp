#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "../Interfaces/IPrimitive.hpp"

namespace RayTracer {

class Sphere : public IPrimitive {
public:
    Sphere();
    Sphere(const Vector3D &center, double radius, const Material &material = Material());
    
    std::optional<HitInfo> intersect(const Ray &ray, double minDist, double maxDist) const override;
    const Material& getMaterial() const override;
    void setMaterial(const Material &material) override;
    std::unique_ptr<IPrimitive> clone() const override;
    const Vector3D& getCenter() const;
    double getRadius() const;
    void setCenter(const Vector3D &center);
    void setRadius(double radius);
    
private:
    Vector3D center;
    double radius;
    Material material;
};

}

#endif // SPHERE_HPP_
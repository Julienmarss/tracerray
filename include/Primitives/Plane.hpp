#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "../Interfaces/IPrimitive.hpp"

namespace RayTracer {

class Plane : public IPrimitive {
public:
    Plane();
    Plane(const Vector3D &normal, double distance, const Material &material = Material());
    Plane(char axis, double position, const Material &material = Material());
    
    std::optional<HitInfo> intersect(const Ray &ray, double minDist, double maxDist) const override;
    const Material& getMaterial() const override;
    void setMaterial(const Material &material) override;
    std::unique_ptr<IPrimitive> clone() const override;
    const Vector3D& getNormal() const;
    double getDistance() const;
    void setNormal(const Vector3D &normal);
    void setDistance(double distance);
    
private:
    Vector3D normal;
    double distance;
    Material material;
    
    static Vector3D axisToNormal(char axis);
};

} // namespace RayTracer

#endif // PLANE_HPP_
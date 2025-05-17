#ifndef CYLINDER_HPP_
#define CYLINDER_HPP_

#include "../Interfaces/IPrimitive.hpp"
#include "../Core/Vector3D.hpp"
#include "../Core/Material.hpp"
#include "../Core/Ray.hpp"
#include <optional>

namespace RayTracer {

class Cylinder : public IPrimitive {
public:
    Cylinder(const Vector3D& position, double radius, double height, const Material& material);

    Vector3D getPosition() const;
    double getRadius() const;
    double getHeight() const;
    const Material& getMaterial() const;

    std::optional<HitInfo> intersect(const Ray &ray, double minDist, double maxDist) const override;
    void setMaterial(const Material &material) override;
    std::unique_ptr<IPrimitive> clone() const override;


private:
    Vector3D _position;
    double _radius;
    double _height;
    Material _material;
};

}

#endif // CYLINDER_HPP_
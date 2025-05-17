#ifndef PLUGIN_CONE_HPP_
#define PLUGIN_CONE_HPP_

#include "../Interfaces/IPrimitive.hpp"
#include "../Core/Vector3D.hpp"
#include "../Core/Material.hpp"
#include <libconfig.h++>

namespace RayTracer {

class Cone : public IPrimitive {
public:
    Cone(const Vector3D& apex, double height, double radius, const Material& mat = Material());
    Cone(const Vector3D& apex, double height, double radius, const Material& mat, bool inverted);
    
    std::optional<HitInfo> intersect(const Ray& ray, double minDist, double maxDist) const override;
    const Material& getMaterial() const override;
    void setMaterial(const Material& material) override;
    std::unique_ptr<IPrimitive> clone() const override;
    
private:
    Vector3D _apex;
    double _height;
    double _radius;
    Material _material;
    bool _inverted;
};

}

extern "C" RayTracer::IPrimitive* create(const libconfig::Setting& config);

#endif // PLUGIN_CONE_HPP_
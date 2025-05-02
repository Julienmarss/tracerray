#ifndef IPRIMITIVE_HPP_
#define IPRIMITIVE_HPP_

#include "../Core/Ray.hpp"
#include "../Core/Material.hpp"
#include "../Core/Vector3D.hpp"
#include <optional>
#include <memory>

namespace RayTracer {

struct HitInfo {
    double distance;
    Vector3D point;
    Vector3D normal;
    Material material;
};

class IPrimitive {
public:
    virtual ~IPrimitive() = default;
    virtual std::optional<HitInfo> intersect(const Ray &ray, double minDist, double maxDist) const = 0;
    virtual const Material& getMaterial() const = 0;
    virtual void setMaterial(const Material &material) = 0;
    virtual std::unique_ptr<IPrimitive> clone() const = 0;
};

}

#endif // IPRIMITIVE_HPP_
#ifndef ILIGHT_HPP_
#define ILIGHT_HPP_

#include "../Core/Vector3D.hpp"
#include "../Core/Color.hpp"
#include <memory>

namespace RayTracer {

class ILight {
public:
    virtual ~ILight() = default;
    virtual Vector3D getDirection(const Vector3D &point) const = 0;
    virtual double getIntensity(const Vector3D &point) const = 0;
    virtual Color getColor() const = 0;
    virtual bool isAmbient() const = 0;
    virtual std::unique_ptr<ILight> clone() const = 0;
};

}

#endif // ILIGHT_HPP_
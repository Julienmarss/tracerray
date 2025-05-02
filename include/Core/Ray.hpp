#ifndef RAY_HPP_
#define RAY_HPP_

#include "Vector3D.hpp"

namespace RayTracer {

class Ray {
public:
    Ray();
    Ray(const Vector3D &origin, const Vector3D &direction);
    
    const Vector3D& getOrigin() const;
    const Vector3D& getDirection() const;
    
    Vector3D pointAt(double t) const;
    
private:
    Vector3D origin;
    Vector3D direction;
};

}

#endif // RAY_HPP_
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "Vector3D.hpp"
#include "Ray.hpp"

namespace RayTracer {

class Camera {
public:
    Camera();
    Camera(const Vector3D &position, const Vector3D &rotation,
           double fieldOfView, int width, int height);
    
    const Vector3D& getPosition() const;
    const Vector3D& getRotation() const;
    double getFieldOfView() const;
    int getWidth() const;
    int getHeight() const;
    
    void setPosition(const Vector3D &position);
    void setRotation(const Vector3D &rotation);
    void setFieldOfView(double fieldOfView);
    void setResolution(int width, int height);
    
    Ray generateRay(int x, int y) const;
    
private:
    Vector3D position;
    Vector3D rotation;
    double fieldOfView;
    int width;
    int height;
};

}

#endif // CAMERA_HPP_
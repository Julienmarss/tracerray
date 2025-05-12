#include "../../include/Core/Camera.hpp"
#include <cmath>
#include <iostream>

RayTracer::Camera::Camera() 
    : position(0, 0, 0), 
      rotation(0, 0, 0), 
      fieldOfView(90.0), 
      width(800), 
      height(600) {}

RayTracer::Camera::Camera(const Vector3D &position, const Vector3D &rotation, 
               double fieldOfView, int width, int height)
    : position(position), 
      rotation(rotation), 
      fieldOfView(fieldOfView), 
      width(width), 
      height(height) {}

const RayTracer::Vector3D& RayTracer::Camera::getPosition() const {
    return position;
}

const RayTracer::Vector3D& RayTracer::Camera::getRotation() const {
    return rotation;
}

double RayTracer::Camera::getFieldOfView() const {
    return fieldOfView;
}

int RayTracer::Camera::getWidth() const {
    return width;
}

int RayTracer::Camera::getHeight() const {
    return height;
}

void RayTracer::Camera::setPosition(const Vector3D &position) {
    this->position = position;
}

void RayTracer::Camera::setRotation(const Vector3D &rotation) {
    this->rotation = rotation;
}

void RayTracer::Camera::setFieldOfView(double fieldOfView) {
    this->fieldOfView = fieldOfView;
}

void RayTracer::Camera::setResolution(int width, int height) {
    this->width = width;
    this->height = height;
}

RayTracer::Ray RayTracer::Camera::generateRay(int x, int y) const {
    double aspect_ratio = static_cast<double>(width) / height;
    double theta = fieldOfView * M_PI / 180.0;
    double viewport_height = 2.0 * tan(theta / 2.0);
    double viewport_width = aspect_ratio * viewport_height;
    Vector3D lookDir(0, 1, 0);
    Vector3D right(1, 0, 0);
    Vector3D up(0, 0, 1);
    double u = static_cast<double>(x) / (width - 1);
    double v = 1.0 - static_cast<double>(y) / (height - 1);
    Vector3D direction = lookDir + 
                        (u - 0.5) * viewport_width * right + 
                        (v - 0.5) * viewport_height * up;
    
    return Ray(position, direction.normalize());
}
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

// Simplified from "Ray Tracing in One Weekend"
RayTracer::Ray RayTracer::Camera::generateRay(int x, int y) const {
    // Debug
    // std::cout << "Debug: Generating ray for pixel (" << x << ", " << y << ")" << std::endl;
    
    // Calculate viewport dimensions based on FOV
    double aspect_ratio = static_cast<double>(width) / height;
    double theta = fieldOfView * M_PI / 180.0;  // Convert FOV to radians
    double viewport_height = 2.0 * tan(theta / 2.0);
    double viewport_width = aspect_ratio * viewport_height;
    
    // Define camera basis vectors (simplified - not using rotation angles yet)
    Vector3D w(0, 1, 0);  // Forward - pointing in +Y direction (for basic setup)
    Vector3D u(1, 0, 0);  // Right - pointing in +X direction
    Vector3D v(0, 0, 1);  // Up - pointing in +Z direction
    
    // Map pixel coordinates to viewport coordinates
    // Pixel (0,0) is top-left, but we want to map it to bottom-left of viewport
    double s = static_cast<double>(x) / (width - 1);
    double t = 1.0 - static_cast<double>(y) / (height - 1);  // Invert Y
    
    // Calculate direction vector
    double px = viewport_width * (s - 0.5);
    double py = 1.0;  // Fixed distance to viewport
    double pz = viewport_height * (t - 0.5);
    
    Vector3D direction = u * px + w * py + v * pz;
    
    // Debug
    // std::cout << "Debug: Ray direction: (" << direction.getX() << ", " 
    //          << direction.getY() << ", " << direction.getZ() << ")" << std::endl;
    
    // Create and return ray
    return Ray(position, direction.normalize());
}
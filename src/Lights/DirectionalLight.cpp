#include "../../include/Lights/DirectionalLight.hpp"
#include <algorithm>

RayTracer::DirectionalLight::DirectionalLight() 
    : direction(0, 0, -1), intensity(1.0), color(Colors::WHITE) {}

RayTracer::DirectionalLight::DirectionalLight(const Vector3D &direction, double intensity, const Color &color)
    : direction(direction.normalize()), 
      intensity(std::clamp(intensity, 0.0, 1.0)), 
      color(color) {}

RayTracer::Vector3D RayTracer::DirectionalLight::getDirection(const Vector3D &point) const {
    (void)point;
    Vector3D lightDir = -direction.normalize();
    return lightDir;
}

double RayTracer::DirectionalLight::getIntensity(const Vector3D &point) const {
    (void)point;
    return intensity;
}

RayTracer::Color RayTracer::DirectionalLight::getColor() const {
    return color;
}

bool RayTracer::DirectionalLight::isAmbient() const {
    return false;
}

std::unique_ptr<RayTracer::ILight> RayTracer::DirectionalLight::clone() const {
    return std::make_unique<DirectionalLight>(*this);
}

const RayTracer::Vector3D& RayTracer::DirectionalLight::getDirectionVector() const {
    return direction;
}

double RayTracer::DirectionalLight::getIntensityValue() const {
    return intensity;
}

void RayTracer::DirectionalLight::setDirection(const Vector3D &direction) {
    this->direction = direction.normalize();
}

void RayTracer::DirectionalLight::setIntensity(double intensity) {
    this->intensity = std::clamp(intensity, 0.0, 1.0);
}

void RayTracer::DirectionalLight::setColor(const Color &color) {
    this->color = color;
}
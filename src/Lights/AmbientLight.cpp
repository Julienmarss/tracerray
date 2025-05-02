#include "../../include/Lights/AmbientLight.hpp"
#include <algorithm>

RayTracer::AmbientLight::AmbientLight() : intensity(1.0), color(Colors::WHITE) {}

RayTracer::AmbientLight::AmbientLight(double intensity, const Color &color)
    : intensity(std::clamp(intensity, 0.0, 1.0)), color(color) {}

RayTracer::Vector3D RayTracer::AmbientLight::getDirection(const Vector3D &point) const {
    (void)point;
    return Vector3D(0, 0, 0);
}

double RayTracer::AmbientLight::getIntensity(const Vector3D &point) const {
    (void)point;
    return intensity;
}

RayTracer::Color RayTracer::AmbientLight::getColor() const {
    return color;
}

bool RayTracer::AmbientLight::isAmbient() const {
    return true;
}

std::unique_ptr<RayTracer::ILight> RayTracer::AmbientLight::clone() const {
    return std::make_unique<AmbientLight>(*this);
}

double RayTracer::AmbientLight::getIntensityValue() const {
    return intensity;
}

void RayTracer::AmbientLight::setIntensity(double intensity) {
    this->intensity = std::clamp(intensity, 0.0, 1.0);
}

void RayTracer::AmbientLight::setColor(const Color &color) {
    this->color = color;
}
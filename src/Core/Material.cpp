#include "../../include/Core/Material.hpp"
#include <algorithm>
#include <iostream>

RayTracer::Material::Material() : color(Colors::WHITE), ambient(0.5), diffuse(0.5) {
/*     std::cout << "Debug: Created default material with ambient=" << ambient 
              << ", diffuse=" << diffuse << std::endl; */
}

RayTracer::Material::Material(const Color &color, double ambient, double diffuse)
    : color(color), 
      ambient(std::clamp(ambient, 0.0, 1.0)), 
      diffuse(std::clamp(diffuse, 0.0, 1.0)) {
/*     std::cout << "Debug: Created material with ambient=" << this->ambient 
              << ", diffuse=" << this->diffuse << std::endl; */
}

const RayTracer::Color& RayTracer::Material::getColor() const {
    return color;
}

double RayTracer::Material::getAmbient() const {
    return ambient;
}

double RayTracer::Material::getDiffuse() const {
    return diffuse;
}

void RayTracer::Material::setColor(const Color &color) {
    this->color = color;
}

void RayTracer::Material::setAmbient(double ambient) {
    this->ambient = std::clamp(ambient, 0.0, 1.0);
}

void RayTracer::Material::setDiffuse(double diffuse) {
    this->diffuse = std::clamp(diffuse, 0.0, 1.0);
}
#include "../../include/Core/Vector3D.hpp"
#include <cmath>
#include <stdexcept>

RayTracer::Vector3D::Vector3D() : x(0), y(0), z(0) {}

RayTracer::Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

RayTracer::Vector3D::Vector3D(const Vector3D &other) : x(other.x), y(other.y), z(other.z) {}

double RayTracer::Vector3D::getX() const {
    return x;
}

double RayTracer::Vector3D::getY() const {
    return y;
}

double RayTracer::Vector3D::getZ() const {
    return z;
}

void RayTracer::Vector3D::setX(double x) {
    this->x = x;
}

void RayTracer::Vector3D::setY(double y) {
    this->y = y;
}

void RayTracer::Vector3D::setZ(double z) {
    this->z = z;
}

RayTracer::Vector3D RayTracer::Vector3D::operator+(const Vector3D &v) const {
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

RayTracer::Vector3D RayTracer::Vector3D::operator-(const Vector3D &v) const {
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

RayTracer::Vector3D RayTracer::Vector3D::operator*(double scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

RayTracer::Vector3D RayTracer::Vector3D::operator/(double scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Division by zero in Vector3D");
    }
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

RayTracer::Vector3D RayTracer::Vector3D::operator-() const {
    return Vector3D(-x, -y, -z);
}

RayTracer::Vector3D& RayTracer::Vector3D::operator=(const Vector3D &other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

double RayTracer::Vector3D::dot(const Vector3D &v) const {
    return x * v.x + y * v.y + z * v.z;
}

RayTracer::Vector3D RayTracer::Vector3D::cross(const Vector3D &v) const {
    return Vector3D(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

double RayTracer::Vector3D::length() const {
    return std::sqrt(lengthSquared());
}

double RayTracer::Vector3D::lengthSquared() const {
    return x * x + y * y + z * z;
}

RayTracer::Vector3D RayTracer::Vector3D::normalize() const {
    double len = length();
    if (len == 0) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    return Vector3D(x / len, y / len, z / len);
}

RayTracer::Vector3D RayTracer::operator*(double scalar, const Vector3D &v) {
    return v * scalar;
}

std::ostream& RayTracer::operator<<(std::ostream &os, const Vector3D &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
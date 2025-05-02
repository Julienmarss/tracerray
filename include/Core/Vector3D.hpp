#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <iostream>

namespace RayTracer {

class Vector3D {
public:
    // Constructors
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(const Vector3D &other);
    
    // Getters
    double getX() const;
    double getY() const;
    double getZ() const;
    
    // Setters
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    
    // Vector operations
    Vector3D operator+(const Vector3D &v) const;
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator*(double scalar) const;
    Vector3D operator/(double scalar) const;
    Vector3D operator-() const;  // Unary negation operator
    
    // Dot product
    double dot(const Vector3D &v) const;
    
    // Cross product
    Vector3D cross(const Vector3D &v) const;
    
    // Length of vector
    double length() const;
    
    // Squared length
    double lengthSquared() const;
    
    // Normalize the vector
    Vector3D normalize() const;
    
    // Assignment operator (to fix the deprecated-copy warning)
    Vector3D& operator=(const Vector3D &other);
    
    // Friend functions
    friend Vector3D operator*(double scalar, const Vector3D &v);
    friend std::ostream& operator<<(std::ostream &os, const Vector3D &v);
    
private:
    double x, y, z;
};

// Déclaration des fonctions amies dans le même espace de noms
Vector3D operator*(double scalar, const Vector3D &v);
std::ostream& operator<<(std::ostream &os, const Vector3D &v);

} // namespace RayTracer

#endif // VECTOR3D_HPP_
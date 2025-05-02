#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "Color.hpp"

namespace RayTracer {

class Material {
public:
    Material();
    Material(const Color &color, double ambient = 1.0, double diffuse = 0.0);
    
    const Color& getColor() const;
    double getAmbient() const;
    double getDiffuse() const;
    
    void setColor(const Color &color);
    void setAmbient(double ambient);
    void setDiffuse(double diffuse);
    
private:
    Color color;
    double ambient;
    double diffuse;
};

}

#endif // MATERIAL_HPP_
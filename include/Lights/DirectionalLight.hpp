#ifndef DIRECTIONAL_LIGHT_HPP_
#define DIRECTIONAL_LIGHT_HPP_

#include "../Interfaces/ILight.hpp"

namespace RayTracer {

class DirectionalLight : public ILight {
public:
    DirectionalLight();
    DirectionalLight(const Vector3D &direction, double intensity = 1.0, const Color &color = Colors::WHITE);
    
    Vector3D getDirection(const Vector3D &point) const override;
    double getIntensity(const Vector3D &point) const override;
    Color getColor() const override;
    bool isAmbient() const override;
    std::unique_ptr<ILight> clone() const override;
    
    const Vector3D& getDirectionVector() const;
    double getIntensityValue() const;
    
    void setDirection(const Vector3D &direction);
    void setIntensity(double intensity);
    void setColor(const Color &color);
    
private:
    Vector3D direction;
    double intensity;
    Color color;
};

}

#endif // DIRECTIONAL_LIGHT_HPP_
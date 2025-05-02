#ifndef AMBIENT_LIGHT_HPP_
#define AMBIENT_LIGHT_HPP_

#include "../Interfaces/ILight.hpp"

namespace RayTracer {

class AmbientLight : public ILight {
public:
    AmbientLight();
    AmbientLight(double intensity, const Color &color = Colors::WHITE);
    
    Vector3D getDirection(const Vector3D &point) const override;
    double getIntensity(const Vector3D &point) const override;
    Color getColor() const override;
    bool isAmbient() const override;
    std::unique_ptr<ILight> clone() const override;
    double getIntensityValue() const;
    void setIntensity(double intensity);
    void setColor(const Color &color);
    
private:
    double intensity;
    Color color;
};

}

#endif // AMBIENT_LIGHT_HPP_
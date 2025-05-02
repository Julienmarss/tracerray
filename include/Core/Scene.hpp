#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "Camera.hpp"
#include "../Interfaces/IPrimitive.hpp"
#include "../Interfaces/ILight.hpp"
#include <vector>
#include <memory>
#include <string>

namespace RayTracer {

class Scene {
public:
    Scene();
    
    void addPrimitive(std::unique_ptr<IPrimitive> primitive);
    void addLight(std::unique_ptr<ILight> light);
    void clear();
    
    const Camera& getCamera() const;
    const std::vector<std::unique_ptr<IPrimitive>>& getPrimitives() const;
    const std::vector<std::unique_ptr<ILight>>& getLights() const;
    
    void setCamera(const Camera &camera);
    std::optional<HitInfo> intersect(const Ray &ray, double minDist, double maxDist) const;
    bool isInShadow(const Vector3D &point, const ILight &light, double maxDist = 1000.0) const;
    
private:
    Camera camera;
    std::vector<std::unique_ptr<IPrimitive>> primitives;
    std::vector<std::unique_ptr<ILight>> lights;
};

}

#endif // SCENE_HPP_
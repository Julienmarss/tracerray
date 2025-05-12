#include "../../include/Core/Scene.hpp"
#include <limits>

RayTracer::Scene::Scene() {}

void RayTracer::Scene::addPrimitive(std::unique_ptr<IPrimitive> primitive) {
    primitives.push_back(std::move(primitive));
}

void RayTracer::Scene::addLight(std::unique_ptr<ILight> light) {
    lights.push_back(std::move(light));
}

void RayTracer::Scene::clear() {
    primitives.clear();
    lights.clear();
}

const RayTracer::Camera& RayTracer::Scene::getCamera() const {
    return camera;
}

const std::vector<std::unique_ptr<RayTracer::IPrimitive>>& RayTracer::Scene::getPrimitives() const {
    return primitives;
}

const std::vector<std::unique_ptr<RayTracer::ILight>>& RayTracer::Scene::getLights() const {
    return lights;
}

void RayTracer::Scene::setCamera(const Camera &camera) {
    this->camera = camera;
}

std::optional<RayTracer::HitInfo> RayTracer::Scene::intersect(const Ray &ray, double minDist, double maxDist) const {
    std::optional<HitInfo> closestHit;
    double closestDistance = maxDist;

    for (const auto &primitive : primitives) {
        auto hit = primitive->intersect(ray, minDist, closestDistance);
        if (hit) {
            closestDistance = hit->distance;
            closestHit = hit;
/*             std::cout << "Debug: Found hit at distance " << hit->distance 
                     << " with material color RGB(" 
                     << (int)hit->material.getColor().getR() << ","
                     << (int)hit->material.getColor().getG() << ","
                     << (int)hit->material.getColor().getB() << ")" << std::endl; */
        }
    }
    
    return closestHit;
}

bool RayTracer::Scene::isInShadow(const Vector3D &point, const ILight &light, double maxDist) const {
    if (light.isAmbient()) {
        return false;
    }
    
    Vector3D lightDir = light.getDirection(point);
    Ray shadowRay(point, lightDir);
    const double EPSILON = 0.001;
    auto hit = intersect(shadowRay, EPSILON, maxDist);
    return hit.has_value();
}
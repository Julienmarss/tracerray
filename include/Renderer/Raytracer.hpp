#ifndef RAYTRACER_HPP_
#define RAYTRACER_HPP_

#include "../Core/Scene.hpp"
#include "../Interfaces/IObserver.hpp"
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>

namespace RayTracer {

class Raytracer {
public:
    Raytracer();
    
    std::vector<Color> render(const Scene &scene);
    void registerObserver(IObserver *observer);
    void removeObserver(IObserver *observer);
    void cancel();
    float getProgress() const;
    void setNumThreads(unsigned int num);
    
private:
    Color traceRay(const Ray &ray, const Scene &scene, int depth = 0) const;
    
    Color calculateLighting(const HitInfo &hitInfo, const Scene &scene) const;
    void notifyObservers(float progress);
    
    std::vector<IObserver*> observers;
    std::atomic<float> progress;
    std::atomic<bool> cancelRequested;

    unsigned int numThreads;
    std::mutex progressMutex;

    static constexpr int MAX_DEPTH = 5;
    static constexpr double MIN_DIST = 0.001;
    static constexpr double MAX_DIST = 1000.0;
};

}

#endif // RAYTRACER_HPP_
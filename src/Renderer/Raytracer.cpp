#include "../../include/Renderer/Raytracer.hpp"
#include <algorithm>
#include <iostream>

RayTracer::Raytracer::Raytracer() : progress(0.0f), cancelRequested(false) {
    numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
}

std::vector<RayTracer::Color> RayTracer::Raytracer::render(const Scene &scene) {
    const Camera &camera = scene.getCamera();
    int width = camera.getWidth();
    int height = camera.getHeight();
    std::vector<Color> image(width * height, Colors::BLACK);
    progress = 0.0f;
    cancelRequested = false;
    int totalPixels = width * height;
    std::atomic<int> pixelsDone(0);
    const int TILE_SIZE = 32;
    int numTilesX = (width + TILE_SIZE - 1) / TILE_SIZE;
    int numTilesY = (height + TILE_SIZE - 1) / TILE_SIZE;
    int totalTiles = numTilesX * numTilesY;
    std::atomic<int> nextTileIndex(0);
    
    std::cout << "\n=== Raytracer Render Start ===" << std::endl;
    std::cout << "Resolution: " << width << "x" << height << " (" << totalPixels << " pixels)" << std::endl;
    std::cout << "Tiles: " << numTilesX << "x" << numTilesY << " (" << totalTiles << " tiles of " << TILE_SIZE << "x" << TILE_SIZE << ")" << std::endl;
    std::cout << "Threads: " << numThreads << std::endl;
    std::cout << "===========================\n" << std::endl;
    
    auto renderTiles = [&]() {
        std::thread::id threadId = std::this_thread::get_id();
        std::cout << "Thread " << threadId << " started" << std::endl;
        int tilesProcessed = 0;
        
        while (true) {
            int tileIndex = nextTileIndex.fetch_add(1);
            if (tileIndex >= totalTiles || cancelRequested) {
                std::cout << "Thread " << threadId << " finished (processed " << tilesProcessed << " tiles)" << std::endl;
                break;
            }
            int tileY = tileIndex / numTilesX;
            int tileX = tileIndex % numTilesX;
            int startX = tileX * TILE_SIZE;
            int startY = tileY * TILE_SIZE;
            int endX = std::min(startX + TILE_SIZE, width);
            int endY = std::min(startY + TILE_SIZE, height);
            
            for (int y = startY; y < endY; ++y) {
                for (int x = startX; x < endX; ++x) {
                    Ray ray = camera.generateRay(x, y);
                    Color color = traceRay(ray, scene);
                    image[y * width + x] = color;
                }
            }
            tilesProcessed++;
            int done = pixelsDone.fetch_add((endX - startX) * (endY - startY));
            float newProgress = static_cast<float>(done) / totalPixels;
            if (newProgress - progress >= 0.005f) {
                std::lock_guard<std::mutex> lock(progressMutex);
                if (newProgress - progress >= 0.005f) {
                    progress = newProgress;
                    notifyObservers(progress);
                }
            }
        }
    };
    std::cout << "Starting " << numThreads << " rendering threads..." << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(renderTiles));
    }
    for (auto &thread : threads) {
        thread.join();
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    if (progress < 1.0f) {
        progress = 1.0f;
        notifyObservers(progress);
    }
    std::cout << "\nAll threads completed" << std::endl;
    std::cout << "Render time: " << duration / 1000.0 << " seconds" << std::endl;
    std::cout << "Average speed: " << (totalPixels * 1000) / duration << " pixels/second" << std::endl;
    std::cout << "===============================" << std::endl;
    return image;
}

void RayTracer::Raytracer::registerObserver(IObserver *observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void RayTracer::Raytracer::removeObserver(IObserver *observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void RayTracer::Raytracer::cancel() {
    cancelRequested = true;
}

float RayTracer::Raytracer::getProgress() const {
    return progress;
}

RayTracer::Color RayTracer::Raytracer::traceRay(const Ray &ray, const Scene &scene, int depth) const {
    if (depth > MAX_DEPTH) {
        return Colors::BLACK;
    }
    
    auto hit = scene.intersect(ray, MIN_DIST, MAX_DIST);
    
    if (hit) {
        return calculateLighting(*hit, scene);
    }
    return Colors::BLACK;
}

RayTracer::Color RayTracer::Raytracer::calculateLighting(const HitInfo &hitInfo, const Scene &scene) const {
    const Material &material = hitInfo.material;
    Color surfaceColor = material.getColor();
    Vector3D normal = hitInfo.normal.normalize();
    Color finalColor = Colors::BLACK;

    for (const auto &light : scene.getLights()) {
        Color lightColor = light->getColor();
        double intensity = light->getIntensity(hitInfo.point);
        
        if (light->isAmbient()) {
            double ambientFactor = material.getAmbient();
            Color ambientColor = surfaceColor * lightColor * (intensity * ambientFactor);
            finalColor = finalColor + ambientColor;
        } 
        else {
            Vector3D lightDir = light->getDirection(hitInfo.point);
            double cosAngle = std::max(0.0, normal.dot(lightDir));
            double diffuseFactor = cosAngle * material.getDiffuse();
            Color diffuseColor = surfaceColor * lightColor * (intensity * diffuseFactor);
            finalColor = finalColor + diffuseColor;
        }
    }
    return finalColor;
}

void RayTracer::Raytracer::notifyObservers(float progress) {
    for (auto observer : observers) {
        observer->update(progress);
    }
}

void RayTracer::Raytracer::setNumThreads(unsigned int num) {
    numThreads = (num > 0) ? num : std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
}
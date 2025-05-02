#include "../../include/Renderer/Raytracer.hpp"
#include <algorithm>
#include <iostream>

RayTracer::Raytracer::Raytracer() : progress(0.0f), cancelRequested(false) {}

std::vector<RayTracer::Color> RayTracer::Raytracer::render(const Scene &scene) {
    // Get camera information
    const Camera &camera = scene.getCamera();
    int width = camera.getWidth();
    int height = camera.getHeight();
    
    // Initialize image data
    std::vector<Color> image(width * height, Colors::BLACK);
    
    // Reset progress and cancel flag
    progress = 0.0f;
    cancelRequested = false;
    
    // Debug info
    std::cout << "Debug: Starting render with size " << width << "x" << height << std::endl;
    std::cout << "Debug: Camera position (" << camera.getPosition().getX() 
              << ", " << camera.getPosition().getY() 
              << ", " << camera.getPosition().getZ() << ")" << std::endl;
    
    // Render each pixel
    int totalPixels = width * height;
    int pixelsDone = 0;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Check if rendering was cancelled
            if (cancelRequested) {
                return image;
            }
            
            // Generate primary ray for this pixel
            Ray ray = camera.generateRay(x, y);
            
            // Trace the ray and get the pixel color
            Color color = traceRay(ray, scene);
            
            // Set the pixel color in the image
            image[y * width + x] = color;
            
            // Update progress
            ++pixelsDone;
            float newProgress = static_cast<float>(pixelsDone) / totalPixels;
            
            // Only notify observers when progress changes significantly
            if (newProgress - progress >= 0.01f) {
                progress = newProgress;
                notifyObservers(progress);
            }
        }
    }
    
    std::cout << "Debug: Finished raytracing" << std::endl;
    
    // Ensure 100% progress is reported
    if (progress < 1.0f) {
        progress = 1.0f;
        notifyObservers(progress);
    }
    
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
    // Base case for recursion
    if (depth > MAX_DEPTH) {
        return Colors::BLACK;
    }
    
    // Check if the ray intersects with any primitive in the scene
    // Use a minimum distance to avoid self-intersection
    auto hit = scene.intersect(ray, MIN_DIST, MAX_DIST);
    
    // If there's an intersection, calculate the color
    if (hit) {
        // Debug
        std::cout << "Debug: Hit found at distance " << hit->distance << std::endl;
        
        return calculateLighting(*hit, scene);
    }
    
    // If there's no intersection, return background color (black)
    return Colors::BLACK;
}

RayTracer::Color RayTracer::Raytracer::calculateLighting(const HitInfo &hitInfo, const Scene &scene) const {
    // Get material properties
    const Material &material = hitInfo.material;
    Color surfaceColor = material.getColor();
    
    // Start with black color
    Color finalColor = Colors::BLACK;
    
    // Debug
    std::cout << "Debug: Surface color: R=" << (int)surfaceColor.getR() 
              << " G=" << (int)surfaceColor.getG() 
              << " B=" << (int)surfaceColor.getB() << std::endl;
    
    // For each light in the scene
    for (const auto &light : scene.getLights()) {
        // Get light properties
        Color lightColor = light->getColor();
        double intensity = light->getIntensity(hitInfo.point);
        
        // Debug
        std::cout << "Debug: Light intensity: " << intensity << std::endl;
        
        // For ambient light, simply add ambient contribution
        if (light->isAmbient()) {
            double ambientFactor = material.getAmbient();
            
            // Debug
            std::cout << "Debug: Ambient factor: " << ambientFactor << std::endl;
            
            // Add ambient contribution
            Color ambientColor = surfaceColor * lightColor * (intensity * ambientFactor);
            finalColor = finalColor + ambientColor;
            
            // Debug
            std::cout << "Debug: Ambient color: R=" << (int)ambientColor.getR() 
                    << " G=" << (int)ambientColor.getG() 
                    << " B=" << (int)ambientColor.getB() << std::endl;
        } 
        // For directional light, calculate diffuse lighting
        else {
            // Get direction to light
            Vector3D lightDir = light->getDirection(hitInfo.point);
            
            // Calculate diffuse lighting using Lambert's cosine law
            double cosAngle = std::max(0.0, hitInfo.normal.dot(lightDir));
            double diffuseFactor = cosAngle * material.getDiffuse();
            
            // Debug
            std::cout << "Debug: Diffuse factor: " << diffuseFactor 
                     << " (cos=" << cosAngle << ", mat=" << material.getDiffuse() << ")" << std::endl;
            
            // Check if the point is in shadow (skip for now)
            // If we want shadows, uncomment this:
            // if (!scene.isInShadow(hitInfo.point, *light)) {
                // Add diffuse contribution
                Color diffuseColor = surfaceColor * lightColor * (intensity * diffuseFactor);
                finalColor = finalColor + diffuseColor;
                
                // Debug
                std::cout << "Debug: Diffuse color: R=" << (int)diffuseColor.getR() 
                        << " G=" << (int)diffuseColor.getG() 
                        << " B=" << (int)diffuseColor.getB() << std::endl;
            // }
        }
    }
    
    // Ensure the final color has some minimum brightness
    if (finalColor.getR() == 0 && finalColor.getG() == 0 && finalColor.getB() == 0) {
        finalColor = surfaceColor * 0.1; // Minimum ambient factor
        
        // Debug
        std::cout << "Debug: Using fallback ambient light" << std::endl;
    }
    
    // Debug final color
    std::cout << "Debug: Final color: R=" << (int)finalColor.getR() 
              << " G=" << (int)finalColor.getG() 
              << " B=" << (int)finalColor.getB() << std::endl;
    
    return finalColor;
}

void RayTracer::Raytracer::notifyObservers(float progress) {
    for (auto observer : observers) {
        observer->update(progress);
    }
}
#include "../../include/Renderer/ProgressObserver.hpp"
#include <iomanip>
#include <chrono>

RayTracer::ProgressObserver::ProgressObserver(const std::string& name) 
    : name(name), lastProgress(0.0f) {
    startTime = std::chrono::high_resolution_clock::now();
}

void RayTracer::ProgressObserver::update(float progress)
{
    if (progress - lastProgress < 0.005f && progress < 1.0f) {
        return;
    }
    lastProgress = progress;
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    double elapsedSeconds = elapsed / 1000.0;
    double estimatedTotal = elapsedSeconds / progress;
    double remaining = estimatedTotal - elapsedSeconds;
    std::cout << "\r";
    const int barWidth = 40;
    int pos = static_cast<int>(barWidth * progress);
    
    std::cout << name << ": [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) 
              << (progress * 100.0f) << "%";
    std::cout << " | " << std::fixed << std::setprecision(1) 
              << elapsedSeconds << "s";
    if (progress > 0.05f && progress < 1.0f) {
        std::cout << " | ETA: " << std::fixed << std::setprecision(1) 
                  << remaining << "s";
    }
    std::cout << std::flush;
    if (progress >= 1.0f) {
        std::cout << " | Done!" << std::endl;
    }
}

void RayTracer::ProgressObserver::setName(const std::string& name) {
    this->name = name;
}
#include "../../include/Renderer/ProgressObserver.hpp"
#include <iomanip>

RayTracer::ProgressObserver::ProgressObserver(const std::string& name) 
    : name(name), lastProgress(0.0f) {}

    void RayTracer::ProgressObserver::update(float progress) {
        if (progress - lastProgress < 0.01f && progress < 1.0f) {
            return;
        }
        
        lastProgress = progress;
        
        std::cout << "\033[2J\033[1;1H";
        
        const int barWidth = 50;
        int pos = static_cast<int>(barWidth * progress);
        
        std::cout << name << ": [";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        
        std::cout << "] " << std::fixed << std::setprecision(1) 
                  << (progress * 100.0f) << "%";
        
        std::cout << std::flush;
        if (progress >= 1.0f) {
            std::cout << std::endl;
        }
    }

void RayTracer::ProgressObserver::setName(const std::string& name) {
    this->name = name;
}
#ifndef PROGRESS_OBSERVER_HPP_
#define PROGRESS_OBSERVER_HPP_

#include "../Interfaces/IObserver.hpp"
#include <iostream>
#include <string>

namespace RayTracer {

class ProgressObserver : public IObserver {
public:
    ProgressObserver(const std::string& name = "Render");
    
    void update(float progress) override;
    void setName(const std::string& name);
    
private:
    std::string name;
    float lastProgress;
};

}

#endif // PROGRESS_OBSERVER_HPP_
#ifndef LIGHT_FACTORY_HPP_
#define LIGHT_FACTORY_HPP_

#include "../Interfaces/ILight.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>

namespace RayTracer {

class LightFactory {
public:
    static std::unique_ptr<ILight> createLight(const libconfig::Setting &config, const std::string &type);
    static std::unique_ptr<ILight> createAmbientLight(const libconfig::Setting &config);
    static std::unique_ptr<ILight> createDirectionalLight(const libconfig::Setting &config);
    static Color createColor(const libconfig::Setting &config);
    
private:
    static bool hasRequiredField(const libconfig::Setting &config, const std::string &field);
};

}

#endif // LIGHT_FACTORY_HPP_
#ifndef PRIMITIVE_FACTORY_HPP_
#define PRIMITIVE_FACTORY_HPP_

#include "../Interfaces/IPrimitive.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>

namespace RayTracer {

class PrimitiveFactory {
public:
    static std::unique_ptr<IPrimitive> createPrimitive(const libconfig::Setting &config);
    static std::unique_ptr<IPrimitive> createSphere(const libconfig::Setting &config);
    static std::unique_ptr<IPrimitive> createPlane(const libconfig::Setting &config);
    static Material createMaterial(const libconfig::Setting &config);
    
private:
    static bool hasRequiredField(const libconfig::Setting &config, const std::string &field);
};

}

#endif // PRIMITIVE_FACTORY_HPP_
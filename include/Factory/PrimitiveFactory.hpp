#ifndef PRIMITIVE_FACTORY_HPP_
#define PRIMITIVE_FACTORY_HPP_

#include "../Interfaces/IPrimitive.hpp"
#include "../Core/Material.hpp"
#include <libconfig.h++>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace RayTracer {

class PrimitiveFactory {
public:
    using Creator = std::function<std::unique_ptr<IPrimitive>(const libconfig::Setting&)>;

    static PrimitiveFactory& getInstance();

    void registerType(const std::string& type, Creator creator);
    std::unique_ptr<IPrimitive> create(const std::string& type, const libconfig::Setting& config) const;

    static Material createMaterial(const libconfig::Setting& config);
    std::unique_ptr<IPrimitive> createPrimitive(const libconfig::Setting& config);

private:
    std::unordered_map<std::string, Creator> _creators;
};

}

#endif // PRIMITIVE_FACTORY_HPP_

/*
** EPITECH PROJECT, 2025
** B-OOP-400-LIL-4-1-raytracer-yanis.asselman
** File description:
** PrimitiveFactory
*/

#include "../../include/Factory/PrimitiveFactory.hpp"
#include "../../include/Core/Color.hpp"
#include <iostream>
#include "../../include/Factory/PluginLoader.hpp"

RayTracer::PrimitiveFactory& RayTracer::PrimitiveFactory::getInstance()
{
    static PrimitiveFactory instance;
    return instance;
}

void RayTracer::PrimitiveFactory::registerType(const std::string& type, Creator creator)
{
    if (type.empty()) {
        std::cerr << "PrimitiveFactory error: Cannot register empty type name" << std::endl;
        return;
    }
    if (!creator) {
        std::cerr << "PrimitiveFactory error: Cannot register null creator for type '" << type << "'" << std::endl;
        return;
    }
    _creators[type] = std::move(creator);
    std::cout << "PrimitiveFactory: Registered type '" << type << "'" << std::endl;
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::PrimitiveFactory::createPrimitive(const libconfig::Setting& config)
{
    try {
        if (config.exists("plugin")) {
            std::string pluginPath = config["plugin"];
            return RayTracer::PluginLoader::load(pluginPath, config);
        }
        std::string type;
        if (config.exists("type")) {
            type = static_cast<const char*>(config["type"]);
        } else {
            if (config.exists("r")) {
                if (config.exists("h")) {
                    type = "cylinder";
                } else {
                    type = "sphere";
                }
            } else if (config.exists("axis") || (config.exists("normal") && config.exists("distance"))) {
                type = "plane";
            } else {
                std::cerr << "Error: Could not determine primitive type" << std::endl;
                return nullptr;
            }
        }
        return create(type, config);
        
    } catch (const libconfig::SettingNotFoundException &e) {
        std::cerr << "Error: Required field not found in primitive config: " << e.getPath() << std::endl;
        return nullptr;
    } catch (const libconfig::SettingTypeException &e) {
        std::cerr << "Error: Type mismatch in primitive config: " << e.getPath() << std::endl;
        return nullptr;
    } catch (const std::exception &e) {
        std::cerr << "Error creating primitive: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::PrimitiveFactory::create(const std::string& type, const libconfig::Setting& config) const
{
    auto it = _creators.find(type);
    if (it == _creators.end()) {
        std::cerr << "PrimitiveFactory error: Unknown type '" << type << "'" << std::endl;
        std::cerr << "Available types: ";
        for (const auto& pair : _creators) {
            std::cerr << "'" << pair.first << "' ";
        }
        std::cerr << std::endl;
        return nullptr;
    }
    try {
        auto primitive = it->second(config);
        if (!primitive) {
            std::cerr << "PrimitiveFactory error: Creator for type '" << type << "' returned null" << std::endl;
        }
        return primitive;
    } catch (const std::exception& e) {
        std::cerr << "PrimitiveFactory error: Exception creating '" << type << "': " << e.what() << std::endl;
        return nullptr;
    }
}

RayTracer::Material RayTracer::PrimitiveFactory::createMaterial(const libconfig::Setting& config)
{
    RayTracer::Color color(static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255)); //par defaut on met en blanc
    double ambient = 0.5;
    double diffuse = 0.5;
    
    try {
        if (config.exists("color")) {
            const auto& col = config["color"];
            if (!col.exists("r") || !col.exists("g") || !col.exists("b")) {
                std::cerr << "Warning: Color requires r, g, b values. Using white." << std::endl;
            } else {
                int r = col["r"];
                int g = col["g"];
                int b = col["b"];
                r = std::max(0, std::min(255, r));
                g = std::max(0, std::min(255, g));
                b = std::max(0, std::min(255, b));
                color = RayTracer::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
            }
        }
        
        if (config.exists("ambient")) {
            ambient = config["ambient"];
            ambient = std::max(0.0, std::min(1.0, ambient));
        }
        if (config.exists("diffuse")) {
            diffuse = config["diffuse"];
            diffuse = std::max(0.0, std::min(1.0, diffuse));
        }
    } catch (const libconfig::SettingException& e) {
        std::cerr << "Warning: Error parsing material: " << e.what() << ". Using defaults." << std::endl;
    }
    return RayTracer::Material(color, ambient, diffuse);
}

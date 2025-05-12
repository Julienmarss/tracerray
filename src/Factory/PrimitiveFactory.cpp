#include "../../include/Factory/PrimitiveFactory.hpp"
#include "../../include/Primitives/Sphere.hpp"
#include "../../include/Primitives/Plane.hpp"
#include <iostream>

std::unique_ptr<RayTracer::IPrimitive> RayTracer::PrimitiveFactory::createPrimitive(const libconfig::Setting &config) {
    try {
        std::string type;
        
        if (config.exists("type")) {
            type = static_cast<const char*>(config["type"]);
        } else {
            if (config.exists("r")) {
                type = "sphere";
            } else if (config.exists("axis")) {
                type = "plane";
            } else {
                std::cerr << "Error: Could not determine primitive type" << std::endl;
                return nullptr;
            }
        }
        if (type == "sphere") {
            return createSphere(config);
        } else if (type == "plane") {
            return createPlane(config);
        } else {
            std::cerr << "Error: Unknown primitive type: " << type << std::endl;
            return nullptr;
        }
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

std::unique_ptr<RayTracer::IPrimitive> RayTracer::PrimitiveFactory::createSphere(const libconfig::Setting &config) {
    if (!hasRequiredField(config, "x") || 
        !hasRequiredField(config, "y") || 
        !hasRequiredField(config, "z") || 
        !hasRequiredField(config, "r")) {
        std::cerr << "Error: Missing required field for sphere" << std::endl;
        return nullptr;
    }
    double x = config["x"];
    double y = config["y"];
    double z = config["z"];
    double radius = config["r"];
    Material material;
    if (config.exists("color")) {
        material = createMaterial(config);
    }
    return std::make_unique<Sphere>(Vector3D(x, y, z), radius, material);
}

std::unique_ptr<RayTracer::IPrimitive> RayTracer::PrimitiveFactory::createPlane(const libconfig::Setting &config) {
    if (hasRequiredField(config, "axis") && hasRequiredField(config, "position")) {
        std::string axisStr = static_cast<const char*>(config["axis"]);
        double position = config["position"];
        if (axisStr.empty()) {
            std::cerr << "Error: Empty axis for plane" << std::endl;
            return nullptr;
        }
        Material material;
        if (config.exists("color")) {
            material = createMaterial(config);
        }
        return std::make_unique<Plane>(axisStr[0], position, material);
    } 
    else if (hasRequiredField(config, "normal") && hasRequiredField(config, "distance")) {
        const libconfig::Setting &normalConfig = config["normal"];
        if (!normalConfig.isList() || normalConfig.getLength() != 3) {
            std::cerr << "Error: Normal must be a list of 3 values" << std::endl;
            return nullptr;
        }
        double nx = normalConfig[0];
        double ny = normalConfig[1];
        double nz = normalConfig[2];
        double distance = config["distance"];
        Material material;
        if (config.exists("color")) {
            material = createMaterial(config);
        }
        return std::make_unique<Plane>(Vector3D(nx, ny, nz), distance, material);
    } else {
        std::cerr << "Error: Invalid plane definition" << std::endl;
        return nullptr;
    }
}

RayTracer::Material RayTracer::PrimitiveFactory::createMaterial(const libconfig::Setting &config) {
    Color color;
    double ambient = 0.5;
    double diffuse = 0.5;
    
    if (config.exists("color")) {
        const libconfig::Setting &colorConfig = config["color"];
        if (colorConfig.exists("r") && colorConfig.exists("g") && colorConfig.exists("b")) {
            int r = colorConfig["r"];
            int g = colorConfig["g"];
            int b = colorConfig["b"];
            color = Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
        }
    }
    if (config.exists("ambient")) {
        ambient = static_cast<double>(config["ambient"]);
    }
    if (config.exists("diffuse")) {
        diffuse = static_cast<double>(config["diffuse"]);
    }
    return Material(color, ambient, diffuse);
}

bool RayTracer::PrimitiveFactory::hasRequiredField(const libconfig::Setting &config, const std::string &field) {
    return config.exists(field);
}
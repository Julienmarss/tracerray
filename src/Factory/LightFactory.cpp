#include "../../include/Factory/LightFactory.hpp"
#include "../../include/Lights/AmbientLight.hpp"
#include "../../include/Lights/DirectionalLight.hpp"
#include <iostream>

std::unique_ptr<RayTracer::ILight> RayTracer::LightFactory::createLight(const libconfig::Setting &config, const std::string &type) {
    try {
        if (type == "ambient") {
            return createAmbientLight(config);
        } else if (type == "directional") {
            return createDirectionalLight(config);
        } else {
            std::cerr << "Error: Unknown light type: " << type << std::endl;
            return nullptr;
        }
    } catch (const libconfig::SettingNotFoundException &e) {
        std::cerr << "Error: Required field not found in light config: " << e.getPath() << std::endl;
        return nullptr;
    } catch (const libconfig::SettingTypeException &e) {
        std::cerr << "Error: Type mismatch in light config: " << e.getPath() << std::endl;
        return nullptr;
    } catch (const std::exception &e) {
        std::cerr << "Error creating light: " << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<RayTracer::ILight> RayTracer::LightFactory::createAmbientLight(const libconfig::Setting &config) {
    double intensity = 1.0;
    Color color = Colors::WHITE;
    
    if (config.exists("intensity")) {
        intensity = static_cast<double>(config["intensity"]);
    }
    if (config.exists("color")) {
        color = createColor(config["color"]);
    }
    return std::make_unique<AmbientLight>(intensity, color);
}

std::unique_ptr<RayTracer::ILight> RayTracer::LightFactory::createDirectionalLight(const libconfig::Setting &config) {
    if (!hasRequiredField(config, "x") || 
        !hasRequiredField(config, "y") || 
        !hasRequiredField(config, "z")) {
        std::cerr << "Error: Missing direction coordinates for directional light" << std::endl;
        return nullptr;
    }
    
    double x = config["x"];
    double y = config["y"];
    double z = config["z"];
    double intensity = 1.0;

    if (config.exists("intensity")) {
        intensity = static_cast<double>(config["intensity"]);
    }
    Color color = Colors::WHITE;
    if (config.exists("color")) {
        color = createColor(config["color"]);
    }
    return std::make_unique<DirectionalLight>(Vector3D(x, y, z), intensity, color);
}

RayTracer::Color RayTracer::LightFactory::createColor(const libconfig::Setting &config) {
    int r = 255, g = 255, b = 255;
    
    if (config.exists("r")) {
        r = static_cast<int>(config["r"]);
    }
    if (config.exists("g")) {
        g = static_cast<int>(config["g"]);
    }
    if (config.exists("b")) {
        b = static_cast<int>(config["b"]);
    }
    return Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
}

bool RayTracer::LightFactory::hasRequiredField(const libconfig::Setting &config, const std::string &field) {
    return config.exists(field);
}
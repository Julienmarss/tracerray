#include "../../include/Parser/SceneParser.hpp"
#include "../../include/Factory/PrimitiveFactory.hpp"
#include "../../include/Factory/LightFactory.hpp"
#include "../../include/Lights/AmbientLight.hpp"
#include <iostream>

RayTracer::SceneParser::SceneParser() {}

bool RayTracer::SceneParser::parseFile(const std::string &filename, Scene &scene) {
    try {
        scene.clear();
        libconfig::Config config;
        config.readFile(filename.c_str());

        if (config.exists("camera")) {
            if (!parseCamera(config.lookup("camera"), scene)) {
                return false;
            }
        } else {
            logError("No camera section found in scene file");
            return false;
        }

        if (config.exists("primitives")) {
            if (!parsePrimitives(config.lookup("primitives"), scene)) {
                return false;
            }
        } else {
            logError("No primitives section found in scene file");
            return false;
        }

        if (config.exists("lights")) {
            if (!parseLights(config.lookup("lights"), scene)) {
                return false;
            }
        } else {
            logError("No lights section found in scene file");
            return false;
        }

        return true;
    } catch (const libconfig::FileIOException &e) {
        logError("Error opening scene file: " + filename);
        return false;
    } catch (const libconfig::ParseException &e) {
        logError("Parse error in scene file " + filename +
                 " at line " + std::to_string(e.getLine()) +
                 ": " + e.getError());
        return false;
    } catch (const libconfig::SettingNotFoundException &e) {
        logError("Required setting not found: " + std::string(e.getPath()));
        return false;
    } catch (const std::exception &e) {
        logError("Error parsing scene file: " + std::string(e.what()));
        return false;
    }
}

bool RayTracer::SceneParser::parseCamera(const libconfig::Setting &config, Scene &scene) {
    try {
        Camera camera;

        if (config.exists("resolution")) {
            const libconfig::Setting &resolution = config["resolution"];
            int width = resolution["width"];
            int height = resolution["height"];
            camera.setResolution(width, height);
        }

        if (config.exists("position")) {
            const libconfig::Setting &position = config["position"];
            double x = static_cast<double>(position["x"]);
            double y = static_cast<double>(position["y"]);
            double z = static_cast<double>(position["z"]);
            camera.setPosition(Vector3D(x, y, z));
        }

        if (config.exists("rotation")) {
            const libconfig::Setting &rotation = config["rotation"];
            double x = static_cast<double>(rotation["x"]);
            double y = static_cast<double>(rotation["y"]);
            double z = static_cast<double>(rotation["z"]);
            camera.setRotation(Vector3D(x, y, z));
        }

        if (config.exists("fieldOfView")) {
            double fov = config["fieldOfView"];
            camera.setFieldOfView(fov);
        }

        scene.setCamera(camera);
        return true;
    } catch (const libconfig::SettingNotFoundException &e) {
        logError("Required camera setting not found: " + std::string(e.getPath()));
        return false;
    } catch (const libconfig::SettingTypeException &e) {
        logError("Type mismatch in camera setting: " + std::string(e.getPath()));
        return false;
    } catch (const std::exception &e) {
        logError("Error parsing camera: " + std::string(e.what()));
        return false;
    }
}

bool RayTracer::SceneParser::parsePrimitives(const libconfig::Setting &config, Scene &scene) {
    try {
        if (config.exists("spheres")) {
            const libconfig::Setting &spheres = config["spheres"];
            for (int i = 0; i < spheres.getLength(); ++i) {
                const libconfig::Setting &sphereConfig = spheres[i];
                auto primitive = PrimitiveFactory::getInstance().create("sphere", sphereConfig);
                if (primitive) {
                    scene.addPrimitive(std::move(primitive));
                } else {
                    logError("Failed to create sphere at index " + std::to_string(i));
                    return false;
                }
            }
        }

        if (config.exists("planes")) {
            const libconfig::Setting &planes = config["planes"];
            for (int i = 0; i < planes.getLength(); ++i) {
                const libconfig::Setting &planeConfig = planes[i];
                auto primitive = PrimitiveFactory::getInstance().create("plane", planeConfig);
                if (primitive) {
                    scene.addPrimitive(std::move(primitive));
                } else {
                    logError("Failed to create plane at index " + std::to_string(i));
                    return false;
                }
            }
        }

        if (config.exists("cylinders")) {
            const libconfig::Setting &cylinders = config["cylinders"];
            for (int i = 0; i < cylinders.getLength(); ++i) {
                const libconfig::Setting &cylinderConfig = cylinders[i];
                auto primitive = PrimitiveFactory::getInstance().create("cylinder", cylinderConfig);
                if (primitive) {
                    scene.addPrimitive(std::move(primitive));
                } else {
                    logError("Failed to create cylinder at index " + std::to_string(i));
                    return false;
                }
            }
        }

        if (config.exists("plugins")) {
            if (!parsePluginPrimitives(config["plugins"], scene)) {
                return false;
            }
        }

        return true;
    } catch (const std::exception &e) {
        logError("Error parsing primitives: " + std::string(e.what()));
        return false;
    }
}

bool RayTracer::SceneParser::parseLights(const libconfig::Setting &config, Scene &scene) {
    try {
        if (config.exists("ambient")) {
            if (!parseAmbientLight(config, scene)) return false;
        }

        if (config.exists("directional")) {
            if (!parseDirectionalLights(config["directional"], scene)) return false;
        }

        if (config.exists("point")) {
            if (!parsePointLights(config["point"], scene)) return false;
        }

        return true;
    } catch (const std::exception &e) {
        logError("Error parsing lights: " + std::string(e.what()));
        return false;
    }
}

bool RayTracer::SceneParser::parseAmbientLight(const libconfig::Setting &config, Scene &scene) {
    try {
        double ambientIntensity = config["ambient"];
        auto ambientLight = std::make_unique<AmbientLight>(ambientIntensity);
        scene.addLight(std::move(ambientLight));
        return true;
    } catch (const std::exception &e) {
        logError("Error parsing ambient light: " + std::string(e.what()));
        return false;
    }
}

bool RayTracer::SceneParser::parseDirectionalLights(const libconfig::Setting &config, Scene &scene) {
    try {
        if (!config.isList()) {
            logError("Directional lights must be a list");
            return false;
        }

        int count = config.getLength();
        for (int i = 0; i < count; ++i) {
            const libconfig::Setting &lightConfig = config[i];
            auto light = LightFactory::createDirectionalLight(lightConfig);
            if (light) {
                scene.addLight(std::move(light));
            } else {
                logError("Failed to create directional light at index " + std::to_string(i));
                return false;
            }
        }

        return true;
    } catch (const std::exception &e) {
        logError("Error parsing directional lights: " + std::string(e.what()));
        return false;
    }
}

bool RayTracer::SceneParser::parsePointLights(const libconfig::Setting &config, Scene &scene) {
    (void)config;
    (void)scene;
    return true;
}

bool RayTracer::SceneParser::hasRequiredField(const libconfig::Setting &config, const std::string &field) {
    return config.exists(field);
}

void RayTracer::SceneParser::logError(const std::string &message) {
    std::cerr << "Scene Parser Error: " << message << std::endl;
}

bool RayTracer::SceneParser::parsePluginPrimitives(const libconfig::Setting &config, Scene &scene) {
    try {
        if (!config.isList()) {
            logError("Plugins must be a list");
            return false;
        }
        int count = config.getLength();
        for (int i = 0; i < count; ++i) {
            const libconfig::Setting &primitiveConfig = config[i];
            auto primitive = PrimitiveFactory::getInstance().createPrimitive(primitiveConfig);
            if (primitive) {
                scene.addPrimitive(std::move(primitive));
            } else {
                logError("Failed to create plugin primitive at index " + std::to_string(i));
                return false;
            }
        }
        return true;
    } catch (const std::exception &e) {
        logError("Error parsing plugin primitives: " + std::string(e.what()));
        return false;
    }
}

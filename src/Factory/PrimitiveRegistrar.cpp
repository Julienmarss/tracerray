#include "../../include/Factory/PrimitiveFactory.hpp"
#include "../../include/Primitives/Sphere.hpp"
#include "../../include/Primitives/Plane.hpp"
#include "../../include/Primitives/Cylinder.hpp"
#include <iostream>

static bool registerPrimitives() {
    RayTracer::PrimitiveFactory::getInstance().registerType("sphere", [](const libconfig::Setting& cfg) -> std::unique_ptr<RayTracer::IPrimitive> {
        try {
            if (!cfg.exists("x") || !cfg.exists("y") || !cfg.exists("z") || !cfg.exists("r")) {
                std::cerr << "Error: Sphere requires x, y, z, and r parameters" << std::endl;
                return nullptr;
            }

            double x = cfg["x"];
            double y = cfg["y"];
            double z = cfg["z"];
            double r = cfg["r"];
            RayTracer::Material mat;
            if (cfg.exists("color")) {
                mat = RayTracer::PrimitiveFactory::createMaterial(cfg);
            }
            return std::make_unique<RayTracer::Sphere>(RayTracer::Vector3D(x, y, z), r, mat);
        } catch (const libconfig::SettingException& e) {
            std::cerr << "Error creating sphere: " << e.what() << std::endl;
            return nullptr;
        }
    });
    
    RayTracer::PrimitiveFactory::getInstance().registerType("plane", [](const libconfig::Setting& cfg) -> std::unique_ptr<RayTracer::IPrimitive> {
        try {
            if (!cfg.exists("axis") || !cfg.exists("position")) {
                std::cerr << "Error: Plane requires axis and position parameters" << std::endl;
                return nullptr;
            }

            std::string axis = cfg["axis"];
            double pos = cfg["position"];

            if (axis.empty()) {
                std::cerr << "Error: Plane axis cannot be empty" << std::endl;
                return nullptr;
            }
            RayTracer::Material mat;
            if (cfg.exists("color")) {
                mat = RayTracer::PrimitiveFactory::createMaterial(cfg);
            }
            return std::make_unique<RayTracer::Plane>(axis[0], pos, mat);
        } catch (const libconfig::SettingException& e) {
            std::cerr << "Error creating plane: " << e.what() << std::endl;
            return nullptr;
        }
    });
    
    RayTracer::PrimitiveFactory::getInstance().registerType("cylinder", [](const libconfig::Setting& cfg) -> std::unique_ptr<RayTracer::IPrimitive> {
        try {
            if (!cfg.exists("x") || !cfg.exists("y") || !cfg.exists("z") || 
                !cfg.exists("r") || !cfg.exists("h")) {
                std::cerr << "Error: Cylinder requires x, y, z, r, and h parameters" << std::endl;
                return nullptr;
            }

            double x = cfg["x"];
            double y = cfg["y"];
            double z = cfg["z"];
            double r = cfg["r"];
            double h = cfg["h"];
            RayTracer::Material mat;
            if (cfg.exists("color")) {
                mat = RayTracer::PrimitiveFactory::createMaterial(cfg);
            }
            return std::make_unique<RayTracer::Cylinder>(RayTracer::Vector3D(x, y, z), r, h, mat);
        } catch (const libconfig::SettingException& e) {
            std::cerr << "Error creating cylinder: " << e.what() << std::endl;
            return nullptr;
        }
    });
    
    std::cout << "PrimitiveFactory: Registered sphere, plane, and cylinder types" << std::endl;
    return true;
}

static bool primitives_registered = registerPrimitives();

/*
** EPITECH PROJECT, 2025
** B-OOP-400-LIL-4-1-raytracer-yanis.asselman
** File description:
** PluginLoader
*/

#include "Factory/PluginLoader.hpp"
#include "Interfaces/IPrimitive.hpp"
#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <libconfig.h++>

std::unique_ptr<RayTracer::IPrimitive> RayTracer::PluginLoader::load(const std::string& path, const libconfig::Setting& config)
{
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "PluginLoader Error: dlopen(" << path << ") failed: " << dlerror() << std::endl;
        return nullptr;
    }
    using CreateFunc = IPrimitive* (*)(const libconfig::Setting&);
    dlerror();
    CreateFunc create = reinterpret_cast<CreateFunc>(dlsym(handle, "create"));
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "PluginLoader Error: dlsym failed: " << dlsym_error << std::endl;
        dlclose(handle);
        return nullptr;
    }
    try {
        return std::unique_ptr<IPrimitive>(create(config));
    } catch (const std::exception& e) {
        std::cerr << "PluginLoader Error: exception during create(): " << e.what() << std::endl;
        dlclose(handle);
        return nullptr;
    }
}

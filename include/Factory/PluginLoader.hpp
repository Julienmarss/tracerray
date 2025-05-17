/*
** EPITECH PROJECT, 2025
** B-OOP-400-LIL-4-1-raytracer-yanis.asselman
** File description:
** PluginLoader
*/

#ifndef PLUGIN_LOADER_HPP_
#define PLUGIN_LOADER_HPP_

#include "Interfaces/IPrimitive.hpp"
#include <libconfig.h++>
#include <memory>
#include <string>

namespace RayTracer {

class PluginLoader {
public:
    static std::unique_ptr<IPrimitive> load(const std::string& path, const libconfig::Setting& config);
};

}

#endif // PLUGIN_LOADER_HPP_
#ifndef IPLUGIN_HPP_
#define IPLUGIN_HPP_

#include <string>

namespace RayTracer {

class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    virtual const std::string& getName() const = 0;
    virtual const std::string& getVersion() const = 0;
    virtual bool initialize() = 0;
    virtual bool shutdown() = 0;
};

typedef IPlugin* (*PluginCreateFunction)();

}

#endif // IPLUGIN_HPP_
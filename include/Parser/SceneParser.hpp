#ifndef SCENE_PARSER_HPP_
#define SCENE_PARSER_HPP_

#include "../Core/Scene.hpp"
#include <libconfig.h++>
#include <string>

namespace RayTracer {

class SceneParser {
public:
    SceneParser();
    
    bool parseFile(const std::string &filename, Scene &scene);
    
private:
    bool parseCamera(const libconfig::Setting &config, Scene &scene);
    bool parsePrimitives(const libconfig::Setting &config, Scene &scene);
    bool parseLights(const libconfig::Setting &config, Scene &scene);
    bool parseSpheres(const libconfig::Setting &config, Scene &scene);
    bool parsePlanes(const libconfig::Setting &config, Scene &scene);
    bool parseAmbientLight(const libconfig::Setting &config, Scene &scene);
    bool parseDirectionalLights(const libconfig::Setting &config, Scene &scene);
    bool parsePointLights(const libconfig::Setting &config, Scene &scene);
    bool hasRequiredField(const libconfig::Setting &config, const std::string &field);
    void logError(const std::string &message);
};

}

#endif // SCENE_PARSER_HPP_
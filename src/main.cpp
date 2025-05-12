#include "../include/Core/Scene.hpp"
#include "../include/Parser/SceneParser.hpp"
#include "../include/Renderer/Raytracer.hpp"
#include "../include/Renderer/PPMWriter.hpp"
#include "../include/Renderer/ProgressObserver.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

void printUsage(const std::string& programName) {
    std::cout << "USAGE: " << programName << " <SCENE_FILE>" << std::endl;
    std::cout << "SCENE_FILE: scene configuration" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        if (argc == 2 && std::string(argv[1]) == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        printUsage(argv[0]);
        return 84;
    }
    RayTracer::Scene scene;
    RayTracer::SceneParser parser;
    try {
        if (!parser.parseFile(argv[1], scene)) {
            std::cerr << "Error: Failed to parse scene file" << std::endl;
            return 84;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    
    RayTracer::Raytracer raytracer;
    RayTracer::ProgressObserver progressObserver("Rendering");
    raytracer.registerObserver(&progressObserver);
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Rendering scene with " << std::thread::hardware_concurrency() 
    << " threads..." << std::endl;
    std::cout << "Rendering scene..." << std::endl;
    std::vector<RayTracer::Color> renderedImage = raytracer.render(scene);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Rendering completed in " << duration / 1000.0 << " seconds" << std::endl;
    std::string inputFile = argv[1];
    std::string outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".ppm";
    std::cout << "Writing image to " << outputFile << "..." << std::endl;
    if (!RayTracer::PPMWriter::writeToFile(outputFile, renderedImage, 
                                          scene.getCamera().getWidth(), 
                                          scene.getCamera().getHeight())) {
        std::cerr << "Error: Failed to write output file" << std::endl;
        return 84;
    }
    std::cout << "Image saved to " << outputFile << std::endl;
    return 0;
}
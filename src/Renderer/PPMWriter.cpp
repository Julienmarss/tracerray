#include "../../include/Renderer/PPMWriter.hpp"
#include <fstream>
#include <iostream>

bool RayTracer::PPMWriter::writeToFile(const std::string &filename, 
                                      const std::vector<Color> &pixels, 
                                      int width, 
                                      int height) {
    try {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open output file: " << filename << std::endl;
            return false;
        }
        
        file << "P3\n";
        file << width << " " << height << "\n";
        file << MAX_COLOR_VALUE << "\n";
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const Color &color = pixels[y * width + x];
                file << static_cast<int>(color.getR()) << " "
                     << static_cast<int>(color.getG()) << " "
                     << static_cast<int>(color.getB()) << " ";
            }
            file << "\n";
        }
        file.close();
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error writing PPM file: " << e.what() << std::endl;
        return false;
    }
}
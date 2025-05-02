#ifndef PPM_WRITER_HPP_
#define PPM_WRITER_HPP_

#include "../Core/Color.hpp"
#include <string>
#include <vector>

namespace RayTracer {

class PPMWriter {
public:
    static bool writeToFile(const std::string &filename, 
                           const std::vector<Color> &pixels, 
                           int width, 
                           int height);
    
private:
    static constexpr int MAX_COLOR_VALUE = 255;
};

}

#endif // PPM_WRITER_HPP_
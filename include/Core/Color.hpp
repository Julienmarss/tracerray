#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <cstdint>

namespace RayTracer {

class Color {
public:
    // Constructors
    Color();
    
    // Constructeur explicite pour uint8_t
    explicit Color(uint8_t r, uint8_t g, uint8_t b);
    
    // Constructeur pour double
    Color(double r, double g, double b);
    
    // Getters
    uint8_t getR() const;
    uint8_t getG() const;
    uint8_t getB() const;
    
    // Color operations
    Color operator+(const Color &c) const;
    Color operator*(double scalar) const;
    Color operator*(const Color &c) const;
    
    // Friend functions
    friend Color operator*(double scalar, const Color &c);
    
private:
    uint8_t r, g, b; // RGB values (0-255)
};

// Déclaration des fonctions amies dans le même espace de noms
Color operator*(double scalar, const Color &c);

// Common colors
namespace Colors {
    extern const Color BLACK;
    extern const Color WHITE;
    extern const Color RED;
    extern const Color GREEN;
    extern const Color BLUE;
}

} // namespace RayTracer

#endif // COLOR_HPP_
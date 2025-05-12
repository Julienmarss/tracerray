#include "../../include/Core/Color.hpp"
#include <algorithm>

RayTracer::Color::Color() : r(0), g(0), b(0) {}

RayTracer::Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

RayTracer::Color::Color(double r, double g, double b)
    : r(static_cast<uint8_t>(std::clamp(r, 0.0, 255.0))),
      g(static_cast<uint8_t>(std::clamp(g, 0.0, 255.0))),
      b(static_cast<uint8_t>(std::clamp(b, 0.0, 255.0))) {}

uint8_t RayTracer::Color::getR() const {
    return r;
}

uint8_t RayTracer::Color::getG() const {
    return g;
}

uint8_t RayTracer::Color::getB() const {
    return b;
}

RayTracer::Color RayTracer::Color::operator+(const Color &c) const {
    return Color(
        static_cast<uint8_t>(std::min(static_cast<int>(r) + c.r, 255)),
        static_cast<uint8_t>(std::min(static_cast<int>(g) + c.g, 255)),
        static_cast<uint8_t>(std::min(static_cast<int>(b) + c.b, 255))
    );
}

RayTracer::Color RayTracer::Color::operator*(double scalar) const {
    scalar = std::clamp(scalar, 0.0, 1.0);
    return Color(
        static_cast<uint8_t>(r * scalar),
        static_cast<uint8_t>(g * scalar),
        static_cast<uint8_t>(b * scalar)
    );
}

RayTracer::Color RayTracer::Color::operator*(const Color &c) const {
    return Color(
        static_cast<uint8_t>((static_cast<int>(r) * c.r) / 255),
        static_cast<uint8_t>((static_cast<int>(g) * c.g) / 255),
        static_cast<uint8_t>((static_cast<int>(b) * c.b) / 255)
    );
}

RayTracer::Color RayTracer::operator*(double scalar, const Color &c) {
    return c * scalar;
}

const RayTracer::Color RayTracer::Colors::BLACK(static_cast<uint8_t>(0), static_cast<uint8_t>(0), static_cast<uint8_t>(0));
const RayTracer::Color RayTracer::Colors::WHITE(static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255));
const RayTracer::Color RayTracer::Colors::RED(static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(0));
const RayTracer::Color RayTracer::Colors::GREEN(static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(0));
const RayTracer::Color RayTracer::Colors::BLUE(static_cast<uint8_t>(0), static_cast<uint8_t>(0), static_cast<uint8_t>(255));
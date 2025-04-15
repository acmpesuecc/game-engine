#ifndef _COLOR_HPP
#define _COLOR_HPP

#include <SDL3/SDL_pixels.h>
namespace Engine {

class Color {
  public:
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    static Color FromFloat(float r, float g, float b, float a = 1.0f) {
        return Color(
            static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255),
            static_cast<uint8_t>(b * 255), static_cast<uint8_t>(a * 255));
    }

    static Color White() { return Color(255, 255, 255); }
    static Color Black() { return Color(0, 0, 0); }
    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Blue() { return Color(0, 0, 255); }
    static Color Transparent() { return Color(0, 0, 0, 0); }

    SDL_Color ToSDLColor() const { return {r, g, b, a}; }
    SDL_FColor ToSDLFColor() const {
        return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    }
};
} // namespace Engine
#endif

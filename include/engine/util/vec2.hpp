#ifndef _VECTOR2_HPP
#define _VECTOR2_HPP
#include <SDL3/SDL_rect.h>
namespace Engine {
class Vector2 {
  public:
    float x = 0.0f;
    float y = 0.0f;

    Vector2() = default;
    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vector2 operator+(const Vector2 &other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2 &other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    SDL_FPoint ToSDLPoint() const { return {x, y}; }

    static Vector2 FromSDLPoint(const SDL_FPoint &point) {
        return Vector2(point.x, point.y);
    }
};
} // namespace Engine
#endif

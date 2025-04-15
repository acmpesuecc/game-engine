#ifndef _RECT_H
#define _RECT_H
#include <SDL3/SDL_rect.h>
namespace Engine {
class Rect {
  public:
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;

    Rect() = default;
    Rect(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    SDL_Rect ToSDLRect() const {
        return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(w),
                static_cast<int>(h)};
    }

    SDL_FRect ToSDLFRect() const { return {x, y, w, h}; }
    static Rect FromSDLFRect(const SDL_FRect &rect) {
        return Rect(rect.x, rect.y, rect.w, rect.h);
    }
};
} // namespace Engine
#endif

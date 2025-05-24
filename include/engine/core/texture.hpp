#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP
#include <engine/core/renderer.hpp>
namespace Engine {
class Texture {
  public:
    Texture() = default;
    ~Texture();
    void SetTexture(SDL_Texture *texture);
    SDL_Texture *GetSDLTexture() const { return m_texture; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

  private:
    SDL_Texture *m_texture = nullptr;
    int m_width = 0;
    int m_height = 0;
};
} // namespace Engine
#endif

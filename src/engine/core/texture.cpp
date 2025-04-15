#include <SDL3/SDL_render.h>
#include <engine/core/texture.hpp>

namespace Engine {
Texture::~Texture() {
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}
} // namespace Engine

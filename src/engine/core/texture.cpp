#include <SDL3/SDL_render.h>
#include <engine/core/texture.hpp>

namespace Engine {
Texture::~Texture() {
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}
void Texture::SetTexture(SDL_Texture *texture) {
    if (texture == nullptr) {
        // TODO: add logging here
        return;
    }
    m_texture = texture;
    m_width = m_texture->w;
    m_height = m_texture->h;
}
} // namespace Engine

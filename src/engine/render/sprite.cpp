#include <engine/core/texture.hpp>
#include <engine/render/renderable.hpp>

namespace Engine {
Sprite::Sprite(std::shared_ptr<Texture> texture, Vector2 pos) {
    SetTexture(texture);
    SetPosition(pos);
}

void Sprite::Render(Renderer &renderer) {
    if (!m_visible || !m_texture || !m_texture->GetSDLTexture()) {
        return;
    }

    SDL_FRect destRect;
    destRect.w = m_sourceRect.w * m_scale.x;
    destRect.h = m_sourceRect.h * m_scale.y;
    destRect.x = m_position.x - (m_pivot.x * destRect.w);
    destRect.y = m_position.y - (m_pivot.y * destRect.h);

    SDL_FPoint pivot = {m_pivot.x * destRect.w, m_pivot.y * destRect.h};
    SDL_FRect sourceRect = m_sourceRect.ToSDLFRect();
    SDL_SetTextureColorMod(m_texture->GetSDLTexture(), m_color.r, m_color.g,
                           m_color.b);
    SDL_SetTextureAlphaMod(m_texture->GetSDLTexture(), m_color.a);

    SDL_RenderTextureRotated(renderer.GetSDLRenderer(),
                             m_texture->GetSDLTexture(), &sourceRect, &destRect,
                             m_rotation, &pivot, (SDL_FlipMode)m_flip);
}

void Sprite::SetTexture(std::shared_ptr<Texture> texture) {
    m_texture = texture;
    if (m_texture) {
        m_sourceRect = {0.0f, 0.0f, (float)m_texture->GetWidth(),
                        (float)m_texture->GetHeight()};
    } else {
        m_sourceRect = {0.0f, 0.0f, 0.0f, 0.0f};
    }
}
} // namespace Engine

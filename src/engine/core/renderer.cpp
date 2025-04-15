#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <engine/core/renderer.hpp>

namespace Engine {
Renderer::~Renderer() { Shutdown(); }
bool Renderer::Init(Window &window) {
    m_window = &window;
    m_renderer = SDL_CreateRenderer(m_window->GetSDLWindow(), 0);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    return m_renderer != nullptr;
}
void Renderer::Shutdown() {
    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}
void Renderer::Clear(Color color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

void Renderer::Present() { SDL_RenderPresent(m_renderer); }

void Renderer::DrawPoint(Vector2 point) {
    SDL_SetRenderDrawColor(m_renderer, m_drawColor.r, m_drawColor.g,
                           m_drawColor.b, m_drawColor.a);
    SDL_RenderPoint(m_renderer, point.x, point.y);
}

void Renderer::DrawLine(Vector2 pos1, Vector2 pos2) {
    SDL_SetRenderDrawColor(m_renderer, m_drawColor.r, m_drawColor.g,
                           m_drawColor.b, m_drawColor.a);
    SDL_RenderLine(m_renderer, pos1.x, pos1.y, pos2.x, pos2.y);
}

void Renderer::DrawRect(Rect rect) {
    SDL_SetRenderDrawColor(m_renderer, m_drawColor.r, m_drawColor.g,
                           m_drawColor.b, m_drawColor.a);
    SDL_FRect frect = rect.ToSDLFRect();
    SDL_RenderRect(m_renderer, &frect);
}

void Renderer::FillRect(Rect rect) {
    SDL_SetRenderDrawColor(m_renderer, m_drawColor.r, m_drawColor.g,
                           m_drawColor.b, m_drawColor.a);
    SDL_FRect frect = rect.ToSDLFRect();
    SDL_RenderFillRect(m_renderer, &frect);
}

void Renderer::SetDrawColor(Color color) {
    m_drawColor = color;
    SDL_SetRenderDrawColor(m_renderer, m_drawColor.r, m_drawColor.g,
                           m_drawColor.b, m_drawColor.a);
}

void Renderer::SetOpacity(float opacity) {
    m_opacity = std::clamp(opacity, 0.0f, 1.0f);
    uint8_t alpha = static_cast<uint8_t>(255.0f * m_opacity);
    m_drawColor.a = alpha;
    SDL_SetRenderDrawColor(m_renderer, m_drawColor.r, m_drawColor.g,
                           m_drawColor.b, m_drawColor.a);
}

void Renderer::SetBlendMode(BlendMode blendMode) {
    m_currentBlendMode = blendMode;
    switch (blendMode) {
    case BlendMode::None:
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
        break;
    case BlendMode::Blend:
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        break;
    case BlendMode::Add:
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_ADD);
        break;
    case BlendMode::Multiply:
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_MUL);
        break;
    }
}

void Renderer::PushBlendMode(BlendMode mode) {
    m_blendModeStack.push_back(m_currentBlendMode);
    SetBlendMode(mode);
}

void Renderer::PopBlendMode() {
    if (!m_blendModeStack.empty()) {
        SetBlendMode(m_blendModeStack.back());
        m_blendModeStack.pop_back();
    }
}

void Renderer::SetViewport(Rect rect) {
    SDL_Rect viewport = rect.ToSDLRect();
    SDL_SetRenderViewport(m_renderer, &viewport);
}

void Renderer::ResetViewport() { SDL_SetRenderViewport(m_renderer, 0); }

} // namespace Engine

#ifndef _RENDERER_HPP
#define _RENDERER_HPP
#include <SDL3/SDL_render.h>
#include <engine/core/window.hpp>
#include <engine/util/color.hpp>
#include <engine/util/rect.hpp>
#include <engine/util/vec2.hpp>
#include <vector>
namespace Engine {
enum class BlendMode { None, Blend, Add, Multiply };
enum class RenderFlip { None, Horizontal, Vertical };
class Renderer {
  public:
    Renderer() = default;
    ~Renderer();

    bool Init(Window &window);
    void Shutdown();

    void Clear(Color color);
    void Present();

    void DrawPoint(Vector2 pos);
    void DrawLine(Vector2 pos1, Vector2 pos2);
    void DrawRect(Rect rect);
    void FillRect(Rect rect);

    void SetDrawColor(Color color);
    Color GetDrawColor() const { return m_drawColor; }
    void SetOpacity(float opacity);
    float GetOpacity() const { return m_opacity; }
    void SetBlendMode(BlendMode blendMode);

    void PushBlendMode(BlendMode mode);
    void PopBlendMode();

    void SetViewport(Rect rect);
    void ResetViewport();

    SDL_Renderer *GetSDLRenderer() const { return m_renderer; }

  private:
    SDL_Renderer *m_renderer = nullptr;
    Window *m_window = nullptr;
    Color m_drawColor = Color::White();
    BlendMode m_currentBlendMode = BlendMode::Blend;
    float m_opacity = 1.0f;
    std::vector<BlendMode> m_blendModeStack;
};
} // namespace Engine
#endif

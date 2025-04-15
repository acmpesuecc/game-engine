#ifndef _LAYER_HPP
#define _LAYER_HPP
#include <algorithm>
#include <engine/core/renderer.hpp>
#include <engine/util/vec2.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
namespace Engine {
class Renderable;
enum class BlendMode;
class Layer {
  public:
    Layer(int layerId, std::string_view name = "");
    ~Layer();

    void Add(std::unique_ptr<Renderable> renderable);
    Renderable *Find(std::string_view name) const;
    bool Remove(Renderable *renderable);
    bool Remove(std::string_view name);
    void Clear();

    void SetLayerName(std::string_view name) { m_name = std::string(name); }

    void SetLayerPosition(Vector2 pos) { m_position = pos; };
    void SetLayerRotation(float angle) { m_rotation = angle; };
    void SetLayerScale(Vector2 scale) { m_scale = scale; };
    void DeltaLayerPosition(Vector2 pos) { m_position = m_position + pos; };
    void DeltaLayerRotation(float angle) { m_rotation += angle; };
    void DeltaLayerScale(Vector2 scale) { m_scale = m_scale + scale; };

    Layer &Position(Vector2 pos);
    Layer &Rotation(float angle);
    Layer &Scale(Vector2 scale);

    void SetVisible(bool visible) { m_visible = visible; };
    void SetOpacity(float opacity) {
        m_opacity = std::clamp(opacity, 0.0f, 1.0f);
    };
    void SetBlendMode(BlendMode mode) { m_blendMode = mode; };

    int GetLayerId() const { return m_layerId; }
    const std::string &GetName() const { return m_name; }
    bool IsVisible() const { return m_visible; }
    float GetOpacity() const { return m_opacity; }
    BlendMode GetBlendMode() const { return m_blendMode; }
    const Vector2 &GetPosition() const { return m_position; }
    float GetRotation() const { return m_rotation; }
    const Vector2 &GetScale() const { return m_scale; }
    std::vector<Renderable *> GetRenderables() const;
    void Render(Renderer &renderer);

  private:
    int m_layerId;
    std::string m_name;
    std::vector<std::unique_ptr<Renderable>> m_renderables;
    std::unordered_map<std::string, Renderable *> m_nameMap;
    Vector2 m_position = Vector2(0.0f, 0.0f);
    float m_rotation = 0.0f;
    Vector2 m_scale = Vector2(1.0f, 1.0f);
    bool m_visible = true;
    float m_opacity = 1.0f;
    BlendMode m_blendMode = BlendMode::Blend;
};
} // namespace Engine
#endif

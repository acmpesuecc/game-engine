#ifndef _RENDERABLE_HPP
#define _RENDERABLE_HPP
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_stdinc.h>
#include <array>
#include <engine/util/color.hpp>
#include <engine/util/rect.hpp>
#include <engine/util/vec2.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#define M_PI 3.14159265358979323846
namespace Engine {
class Renderer;
class Texture;
enum class Flip { None, Horizontal, Vertical };
enum class RenderableType { Sprite, Text, Rectangle, Line, Triangle, Circle };
class Texture;

class Renderable {
  public:
    Renderable() = default;
    virtual ~Renderable() = default;

    virtual void Render(Renderer &renderer) = 0;
    virtual RenderableType GetType() const = 0;

    void SetPosition(Vector2 pos) { m_position = pos; }
    Vector2 GetPosition() const { return m_position; }

    void TranslateX(float dx) { m_position.x += dx; }
    void TranslateY(float dy) { m_position.y += dy; }
    void Move(Vector2 delta) { m_position = m_position + delta; }

    void SetRotation(float angleDegrees) { m_rotation = angleDegrees; }
    float GetRotation() const { return m_rotation; }
    void Rotate(float deltaDegrees) { m_rotation += deltaDegrees; }

    void SetScale(Vector2 scale) { m_scale = scale; }
    Vector2 GetScale() const { return m_scale; }
    void Scale(Vector2 scale) {
        m_scale.x *= scale.x;
        m_scale.y *= scale.y;
    }
    void Scale(float scale) { Scale({scale, scale}); }

    void SetPivot(Vector2 pivot) { m_pivot = pivot; }
    Vector2 GetPivot() const { return m_pivot; }

    void SetColor(Color color) { m_color = color; }
    Color GetColor() const { return m_color; }

    void SetName(std::string_view name) { m_name = name; }
    const std::string &GetName() const { return m_name; }

    void SetVisible(bool visible) { m_visible = visible; }
    bool IsVisible() const { return m_visible; }

  protected:
    Vector2 m_position = Vector2(0.0f, 0.0f);
    float m_rotation = 0.0f;
    Vector2 m_scale = Vector2(1.0f, 1.0f);
    Vector2 m_pivot = Vector2(0.5f, 0.5f);
    Color m_color = Color::White();
    bool m_visible = true;
    std::string m_name;
};

class Sprite : public Renderable {
  public:
    Sprite(std::shared_ptr<Texture> texture, Vector2 pos = Vector2(0.0f, 0.0f));

    void Render(Renderer &renderer) override;
    RenderableType GetType() const override { return RenderableType::Sprite; }

    void SetTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetTexture() const { return m_texture; }

    void SetSourceRect(const Rect &rect) { m_sourceRect = rect; }
    Rect GetSourceRect() const { return m_sourceRect; }

    void SetFlip(Flip flip) { m_flip = flip; }
    Flip GetFlip() const { return m_flip; }

  private:
    std::shared_ptr<Texture> m_texture = nullptr;
    Rect m_sourceRect = Rect(0.0f, 0.0f, 0.0f, 0.0f);
    Flip m_flip = Flip::None;
};

class RectangleShape : public Renderable {
  public:
    RectangleShape(Rect rect, Color color, bool filled = true);

    void Render(Renderer &renderer) override;

    RenderableType GetType() const override {
        return RenderableType::Rectangle;
    }

    void SetDimensions(float width, float height) {
        m_width = width;
        m_height = height;
    }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    void SetFilled(bool filled) { m_filled = filled; }
    bool IsFilled() const { return m_filled; }

  private:
    float m_width = 0.0f;
    float m_height = 0.0f;
    bool m_filled = true;
};

class Line : public Renderable {
  public:
    Line(Vector2 start, Vector2 end, Color color);

    void Render(Renderer &renderer) override;

    RenderableType GetType() const override { return RenderableType::Line; }

    void SetRelativeEndPoint(Vector2 delta) { m_relativeEndPoint = delta; }
    Vector2 GetRelativeEndPoint() const { return m_relativeEndPoint; }

    void SetAbsoluteEndPoint(Vector2 end) {
        m_relativeEndPoint = end - m_position;
    }
    Vector2 GetAbsoluteEndPoint();

  private:
    Vector2 m_relativeEndPoint = Vector2(0.0f, 0.0f);
};

class TriangleShape : public Renderable {
  public:
    TriangleShape(Vector2 pos1, Vector2 pos2, Vector2 pos3, Color color,
                  bool filled = false);

    void Render(Renderer &renderer) override;

    RenderableType GetType() const override { return RenderableType::Triangle; }

    void SetVertices(Vector2 pos1, Vector2 pos2, Vector2 pos3);

    std::array<Vector2, 3> GetAbsoluteVertices() const {
        return {CalculateAbsoluteVertex(m_relVertex1),
                CalculateAbsoluteVertex(m_relVertex2),
                CalculateAbsoluteVertex(m_relVertex3)};
    }

    void SetFilled(bool filled) { m_filled = filled; }
    bool IsFilled() const { return m_filled; }

  private:
    Vector2 m_relVertex1, m_relVertex2, m_relVertex3;
    bool m_filled = false;

    Vector2 CalculateAbsoluteVertex(const Vector2 &relVertex) const {
        float rad = m_rotation * (M_PI / 180.0f);
        float cosTheta = SDL_cosf(rad);
        float sinTheta = SDL_sinf(rad);

        return {m_position.x + (relVertex.x * m_scale.x * cosTheta -
                                relVertex.y * m_scale.y * sinTheta),
                m_position.y + (relVertex.x * m_scale.x * sinTheta +
                                relVertex.y * m_scale.y * cosTheta)};
    }
};

class CircleShape : public Renderable {
  public:
    CircleShape(Vector2 pos, float radius, Color color, bool filled = false);

    void Render(Renderer &renderer) override;
    RenderableType GetType() const override { return RenderableType::Circle; }

    void SetRadius(float radius) { m_radius = radius; }
    float GetRadius() const { return m_radius; }

    void SetFilled(bool filled) { m_filled = filled; }
    bool IsFilled() const { return m_filled; }

  private:
    float m_radius = 0.0f;
    bool m_filled = false;
    Vector2 TransformPoint(float x, float y) const;
    std::vector<Vector2> GetCirclePoints(int segments) const;
};
} // namespace Engine
#endif

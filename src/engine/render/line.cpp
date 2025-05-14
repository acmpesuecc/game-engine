#include "engine/util/vec2.hpp"
#include <engine/core/renderer.hpp>
#include <engine/render/renderable.hpp>

namespace Engine {
Line::Line(Vector2 start, Vector2 end, Color color) {
    SetPosition(start);
    m_relativeEndPoint = end - start;
    SetColor(color);
    SetPivot(Vector2(0.0f, 0.0f));
}

void Line::Render(Renderer &renderer) {
    if (!m_visible) {
        return;
    }

    renderer.SetDrawColor(m_color);

    float rad = m_rotation * (M_PI / 180.0f);
    float cosTheta = SDL_cosf(rad);
    float sinTheta = SDL_sinf(rad);

    float scaledEndX = m_relativeEndPoint.x * m_scale.x;
    float scaledEndY = m_relativeEndPoint.y * m_scale.y;

    float rotatedEndX = scaledEndX * cosTheta - scaledEndY * sinTheta;
    float rotatedEndY = scaledEndX * sinTheta + scaledEndY * cosTheta;

    Vector2 absoluteEndPoint =
        Vector2(m_position.x + rotatedEndX, m_position.y + rotatedEndY);

    renderer.DrawLine(m_position, absoluteEndPoint);
}

Vector2 Line::GetAbsoluteEndPoint() {
    float rad = m_rotation * (M_PI / 180.0f);
    float cosTheta = SDL_cosf(rad);
    float sinTheta = SDL_sinf(rad);

    float scaledEndX = m_relativeEndPoint.x * m_scale.x;
    float scaledEndY = m_relativeEndPoint.y * m_scale.y;

    return Vector2(m_position.x + scaledEndX * cosTheta - scaledEndY * sinTheta,
                   m_position.y + scaledEndX * sinTheta +
                       scaledEndY * cosTheta);
}
} // namespace Engine

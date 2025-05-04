#include "engine/util/vec2.hpp"
#include <engine/core/renderer.hpp>
#include <engine/render/renderable.hpp>

namespace Engine {
CircleShape::CircleShape(Vector2 pos, float radius, Color color, bool filled) {
    m_filled = filled;
    m_radius = radius;
    SetPosition(pos);
    SetColor(color);
    SetPivot(Vector2(0.5f, 0.5f));
}

void CircleShape::Render(Renderer &renderer) {
    if (!m_visible) {
        return;
    }

    const int CIRCLE_SEGMENTS = 36;
    renderer.SetDrawColor(m_color);

    std::vector<Vector2> perimeterPoints = GetCirclePoints(CIRCLE_SEGMENTS);

    if (m_filled) {
        std::vector<SDL_Vertex> vertices;
        vertices.reserve(CIRCLE_SEGMENTS + 2);

        SDL_FColor color = m_color.ToSDLFColor();

        Vector2 center = TransformPoint(0, 0);
        vertices.push_back({center.ToSDLPoint(), color, {0.5f, 0.5f}});

        for (const Vector2 &point : perimeterPoints) {
            float normalizedX =
                (point.x - center.x) / (m_radius * m_scale.x) * 0.5f + 0.5f;
            float normalizedY =
                (point.y - center.y) / (m_radius * m_scale.y) * 0.5f + 0.5f;
            vertices.push_back(
                {point.ToSDLPoint(), color, {normalizedX, normalizedY}});
        }

        std::vector<int> indices;
        indices.reserve(CIRCLE_SEGMENTS * 3);

        for (int i = 1; i <= CIRCLE_SEGMENTS; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i < CIRCLE_SEGMENTS ? i + 1 : 1);
        }

        SDL_RenderGeometry(renderer.GetSDLRenderer(), nullptr, vertices.data(),
                           vertices.size(), indices.data(), indices.size());
    } else {
        for (int i = 0; i < CIRCLE_SEGMENTS; i++) {
            int nextIdx = (i + 1) % CIRCLE_SEGMENTS;
            renderer.DrawLine(perimeterPoints[i], perimeterPoints[nextIdx]);
        }
    }
}

Vector2 CircleShape::TransformPoint(float x, float y) const {
    float rotationRad = m_rotation * (M_PI / 180.0f);
    float sinAngle = SDL_sinf(rotationRad);
    float cosAngle = SDL_cosf(rotationRad);

    float pivotOffsetX = 2 * m_radius * m_scale.x * m_pivot.x;
    float pivotOffsetY = 2 * m_radius * m_scale.y * m_pivot.y;

    float scaledX = x * m_scale.x;
    float scaledY = y * m_scale.y;

    scaledX -= pivotOffsetX;
    scaledY -= pivotOffsetY;

    float rotX = scaledX * cosAngle - scaledY * sinAngle;
    float rotY = scaledX * sinAngle + scaledY * cosAngle;

    float adjustedPivotX = pivotOffsetX * cosAngle - pivotOffsetY * sinAngle;
    float adjustedPivotY = pivotOffsetX * sinAngle + pivotOffsetY * cosAngle;

    return Vector2(m_position.x + rotX + adjustedPivotX,
                   m_position.y + rotY + adjustedPivotY);
}

std::vector<Vector2> CircleShape::GetCirclePoints(int segments) const {
    std::vector<Vector2> points;
    points.reserve(segments);

    for (int i = 0; i < segments; i++) {
        float angle = (i * 2.0f * M_PI) / segments;
        points.push_back(TransformPoint(SDL_cosf(angle) * m_radius,
                                        SDL_sinf(angle) * m_radius));
    }

    return points;
}
} // namespace Engine

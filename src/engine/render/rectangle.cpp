#include "engine/util/vec2.hpp"
#include <engine/core/renderer.hpp>
#include <engine/render/renderable.hpp>

namespace Engine {
RectangleShape::RectangleShape(Rect rect, Color color, bool filled) {
    m_width = rect.w;
    m_height = rect.h;
    m_filled = filled;
    SetPosition(Vector2(rect.x, rect.y));
    SetColor(color);
    SetPivot(Vector2(0.0f, 0.0f));
}

void RectangleShape::Render(Renderer &renderer) {
    if (!m_visible) {
        return;
    }

    renderer.SetDrawColor(m_color);
    if (m_rotation == 0.0f) {
        Rect rect;
        rect.w = m_width * m_scale.x;
        rect.h = m_height * m_scale.y;
        rect.x = m_position.x - (m_pivot.x * rect.w);
        rect.y = m_position.y - (m_pivot.y * rect.h);

        if (m_filled) {
            renderer.FillRect(rect);
        } else {
            renderer.DrawRect(rect);
        }
    } else {
        float thetaRad = m_rotation * (M_PI / 180.0f);
        float cosTheta = SDL_cosf(thetaRad);
        float sinTheta = SDL_sinf(thetaRad);
        float scaledWidth = m_width * m_scale.x;
        float scaledHeight = m_height * m_scale.y;
        float pivotOffsetX = m_pivot.x * scaledWidth;
        float pivotOffsetY = m_pivot.y * scaledHeight;
        SDL_FPoint offsets[4] = {
            {-pivotOffsetX, -pivotOffsetY},
            {scaledWidth - pivotOffsetX, -pivotOffsetY},
            {scaledWidth - pivotOffsetX, scaledHeight - pivotOffsetY},
            {-pivotOffsetX, scaledHeight - pivotOffsetY}};
        if (m_filled) {
            SDL_Vertex vertices[4];
            int indices[6] = {0, 1, 2, 0, 2, 3};
            for (int i = 0; i < 4; ++i) {
                float rotatedOffsetX =
                    offsets[i].x * cosTheta - offsets[i].y * sinTheta;
                float rotatedOffsetY =
                    offsets[i].x * sinTheta + offsets[i].y * cosTheta;

                vertices[i].position = {m_position.x + rotatedOffsetX,
                                        m_position.y + rotatedOffsetY};
                vertices[i].color = {
                    (float)m_color.r / 255.0f, (float)m_color.g / 255.0f,
                    (float)m_color.b / 255.0f, (float)m_color.a / 255.0f};
                vertices[i].tex_coord = {0.0f, 0.0f};
            }

            SDL_RenderGeometry(renderer.GetSDLRenderer(), nullptr, vertices, 4,
                               indices, 6);
        } else {
            renderer.SetDrawColor(m_color);
            renderer.DrawLine(
                Vector2(m_position.x +
                            (offsets[0].x * cosTheta - offsets[0].y * sinTheta),
                        m_position.y + (offsets[0].x * sinTheta +
                                        offsets[0].y * cosTheta)),
                Vector2(m_position.x +
                            (offsets[1].x * cosTheta - offsets[1].y * sinTheta),
                        m_position.y + (offsets[1].x * sinTheta +
                                        offsets[1].y * cosTheta)));
            renderer.DrawLine(
                Vector2(m_position.x +
                            (offsets[1].x * cosTheta - offsets[1].y * sinTheta),
                        m_position.y + (offsets[1].x * sinTheta +
                                        offsets[1].y * cosTheta)),
                Vector2(m_position.x +
                            (offsets[2].x * cosTheta - offsets[2].y * sinTheta),
                        m_position.y + (offsets[2].x * sinTheta +
                                        offsets[2].y * cosTheta)));
            renderer.DrawLine(
                Vector2(m_position.x +
                            (offsets[2].x * cosTheta - offsets[2].y * sinTheta),
                        m_position.y + (offsets[2].x * sinTheta +
                                        offsets[2].y * cosTheta)),
                Vector2(m_position.x +
                            (offsets[3].x * cosTheta - offsets[3].y * sinTheta),
                        m_position.y + (offsets[3].x * sinTheta +
                                        offsets[3].y * cosTheta)));
            renderer.DrawLine(
                Vector2(m_position.x +
                            (offsets[0].x * cosTheta - offsets[0].y * sinTheta),
                        m_position.y + (offsets[0].x * sinTheta +
                                        offsets[0].y * cosTheta)),
                Vector2(m_position.x +
                            (offsets[3].x * cosTheta - offsets[3].y * sinTheta),
                        m_position.y + (offsets[3].x * sinTheta +
                                        offsets[3].y * cosTheta)));
        }
    }
}
} // namespace Engine

#include "engine/core/renderer.hpp"
#include "engine/util/vec2.hpp"
#include <engine/render/renderable.hpp>

namespace Engine {
TriangleShape::TriangleShape(Vector2 pos1, Vector2 pos2, Vector2 pos3,
                             Color color, bool filled) {
    m_filled = filled;
    SetVertices(pos1, pos2, pos3);
    SetColor(color);
    SetPivot(Vector2(0.5f, 0.5f));
}

void TriangleShape::SetVertices(Vector2 pos1, Vector2 pos2, Vector2 pos3) {
    m_position.x = (pos1.x + pos2.x + pos3.x) / 3.0f;
    m_position.y = (pos1.y + pos2.y + pos3.y) / 3.0f;

    m_relVertex1 = pos1 - m_position;
    m_relVertex2 = pos2 - m_position;
    m_relVertex3 = pos3 - m_position;
}

void TriangleShape::Render(Renderer &renderer) {
    if (!m_visible) {
        return;
    }
    std::array<Vector2, 3> vertices = GetAbsoluteVertices();
    renderer.SetDrawColor(m_color);
    if (m_filled) {
        SDL_Vertex drawVertices[3];
        int indices[3] = {0, 1, 2};
        for (int i = 0; i < 3; i++) {
            drawVertices[i].position = vertices[i].ToSDLPoint();
            drawVertices[i].color = m_color.ToSDLFColor();
            drawVertices[i].tex_coord = {0.0f, 0.0f};
        }
        SDL_RenderGeometry(renderer.GetSDLRenderer(), nullptr, drawVertices, 3,
                           indices, 3);
    } else {
        renderer.DrawLine(vertices[0], vertices[1]);
        renderer.DrawLine(vertices[1], vertices[2]);
        renderer.DrawLine(vertices[0], vertices[2]);
    }
}
} // namespace Engine

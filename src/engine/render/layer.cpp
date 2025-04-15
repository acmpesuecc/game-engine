#include <engine/core/renderer.hpp>
#include <engine/render/layer.hpp>
#include <engine/render/renderable.hpp>

namespace Engine {
Layer::Layer(int layerId, std::string_view name) {
    m_layerId = layerId;
    m_name = name;
}

Layer::~Layer() { Clear(); }

void Layer::Add(std::unique_ptr<Renderable> renderable) {
    if (!renderable) {
        return;
    }
    const std::string &name = renderable->GetName();
    if (!name.empty()) {
        m_nameMap[name] = renderable.get();
    }
    m_renderables.push_back(std::move(renderable));
}

Renderable *Layer::Find(std::string_view name) const {
    if (name.empty()) {
        return nullptr;
    }
    auto it = m_nameMap.find(std::string(name));
    return (it != m_nameMap.end()) ? it->second : nullptr;
}

bool Layer::Remove(std::string_view name) {
    if (name.empty()) {
        return false;
    }
    auto it = m_nameMap.find(std::string(name));
    if (it != m_nameMap.end()) {
        Renderable *target = it->second;
        m_nameMap.erase(it);
        auto rIt =
            std::find_if(m_renderables.begin(), m_renderables.end(),
                         [target](const auto &r) { return r.get() == target; });
        if (rIt != m_renderables.end()) {
            m_renderables.erase(rIt);
            return true;
        }
    }
    return false;
}

bool Layer::Remove(Renderable *renderable) {
    if (!renderable) {
        return false;
    }
    const std::string &name = renderable->GetName();
    if (!name.empty()) {
        m_nameMap.erase(name);
    }
    auto it = std::find_if(
        m_renderables.begin(), m_renderables.end(),
        [renderable](const auto &r) { return r.get() == renderable; });
    if (it != m_renderables.end()) {
        m_renderables.erase(it);
        return true;
    }
    return false;
}

void Layer::Clear() {
    m_renderables.clear();
    m_nameMap.clear();
}

Layer &Layer::Position(Vector2 pos) {
    SetLayerPosition(pos);
    return *this;
}

Layer &Layer::Rotation(float degrees) {
    SetLayerRotation(degrees);
    return *this;
}

Layer &Layer::Scale(Vector2 scale) {
    SetLayerScale(scale);
    return *this;
}

std::vector<Renderable *> Layer::GetRenderables() const {
    std::vector<Renderable *> result;
    result.reserve(m_renderables.size());

    for (const auto &renderable : m_renderables) {
        result.push_back(renderable.get());
    }

    return result;
}

void Layer::Render(Renderer &renderer) {
    if (!m_visible || m_renderables.empty())
        return;

    Color prevColor = renderer.GetDrawColor();
    float prevOpacity = renderer.GetOpacity();
    renderer.PushBlendMode(m_blendMode);

    renderer.SetOpacity(m_opacity * prevOpacity);

    for (auto &renderable : m_renderables) {
        if (!renderable->IsVisible())
            continue;

        Color origColor = renderable->GetColor();
        Color adjustedColor = origColor;
        adjustedColor.a = static_cast<uint8_t>(origColor.a * m_opacity);

        Vector2 origPos = renderable->GetPosition();
        float origRot = renderable->GetRotation();
        Vector2 origScale = renderable->GetScale();

        float rad = m_rotation * (M_PI / 180.0f);
        float cosR = SDL_cosf(rad);
        float sinR = SDL_sinf(rad);
        Vector2 rotatedPos(origPos.x * cosR - origPos.y * sinR,
                           origPos.x * sinR + origPos.y * cosR);

        rotatedPos.x *= m_scale.x;
        rotatedPos.y *= m_scale.y;
        Vector2 finalPos = rotatedPos + m_position;

        renderable->SetPosition(finalPos);
        renderable->SetRotation(origRot + m_rotation);
        renderable->SetScale(
            Vector2(origScale.x * m_scale.x, origScale.y * m_scale.y));
        renderable->SetColor(adjustedColor);

        renderable->Render(renderer);

        renderable->SetPosition(origPos);
        renderable->SetRotation(origRot);
        renderable->SetScale(origScale);
        renderable->SetColor(origColor);
    }

    renderer.PopBlendMode();
    renderer.SetOpacity(prevOpacity);
    renderer.SetDrawColor(prevColor);
}
} // namespace Engine

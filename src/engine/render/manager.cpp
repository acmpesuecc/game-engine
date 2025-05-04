#include <engine/render/manager.hpp>
#include <engine/render/renderable.hpp>

namespace Engine {

std::unordered_map<int, std::string> RenderManager::s_layerNames = {
    {Layers::BACKGROUND, "Background"},
    {Layers::WORLD, "World"},
    {Layers::ENTITIES, "Entities"},
    {Layers::FOREGROUND, "Foreground"},
    {Layers::UI, "UI"},
    {Layers::DEBUG, "Debug"}};

int RenderManager::s_nextCustomLayerId = Layers::USER_DEFINED_START;

RenderManager::~RenderManager() { Clear(); }

Layer &RenderManager::GetLayer(int layerId) {
    auto it = m_layers.find(layerId);
    if (it == m_layers.end()) {
        std::string layerName;
        auto nameIt = s_layerNames.find(layerId);
        if (nameIt != s_layerNames.end()) {
            layerName = nameIt->second;
        }

        auto [newIt, inserted] = m_layers.emplace(
            std::piecewise_construct, std::forward_as_tuple(layerId),
            std::forward_as_tuple(layerId, layerName));
        return newIt->second;
    }
    return it->second;
}

bool RenderManager::HasLayer(int layerId) const {
    return m_layers.find(layerId) != m_layers.end();
}

void RenderManager::AddRenderable(std::unique_ptr<Renderable> renderable,
                                  int layerId) {
    if (!renderable) {
        return;
    }
    GetLayer(layerId).Add(std::move(renderable));
}

bool RenderManager::RemoveRenderable(std::string_view name) {
    if (name.empty()) {
        return false;
    }
    for (auto &[id, layer] : m_layers) {
        if (layer.Remove(name)) {
            return true;
        }
    }
    return false;
}

Renderable *RenderManager::GetRenderable(std::string_view name) {
    if (name.empty()) {
        return nullptr;
    }
    for (auto &[id, layer] : m_layers) {
        Renderable *renderable = layer.Find(name);
        if (renderable) {
            return renderable;
        }
    }
    return nullptr;
}

Renderable *RenderManager::GetRenderableInLayer(std::string_view name,
                                                int layerId) {
    auto it = m_layers.find(layerId);
    if (it != m_layers.end()) {
        return it->second.Find(name);
    }
    return nullptr;
}

std::vector<Renderable *> RenderManager::GetRenderablesInLayer(int layerId) {
    std::vector<Renderable *> result;

    auto it = m_layers.find(layerId);
    if (it != m_layers.end()) {
        const auto &layerRenderables = it->second.GetRenderables();
        result.reserve(layerRenderables.size());

        for (Renderable *renderable : layerRenderables) {
            result.push_back(renderable);
        }
    }

    return result;
}

int RenderManager::CreateCustomLayer(int layerInFront) {
    int newLayerId = s_nextCustomLayerId++;
    if (layerInFront != -1) {
        newLayerId = layerInFront - 1;
    }
    return newLayerId;
}

int RenderManager::CreateCustomLayer(std::string_view layerName,
                                     int layerInFront) {
    int newLayerId = CreateCustomLayer(layerInFront);
    RegisterLayerName(newLayerId, layerName);
    return newLayerId;
}

void RenderManager::RegisterLayerName(int layerId, std::string_view layerName) {
    s_layerNames[layerId] = std::string(layerName);

    auto it = m_layers.find(layerId);
    if (it != m_layers.end()) {
        it->second.SetLayerName(std::string(layerName));
    }
}

std::string_view RenderManager::GetLayerName(int layerId) {
    static const std::string empty_string = "";
    auto it = s_layerNames.find(layerId);
    return (it != s_layerNames.end()) ? it->second : empty_string;
}

void RenderManager::CreateLayerGroup(std::string_view groupName,
                                     const std::vector<int> &layerIds) {
    m_layerGroups[std::string(groupName)] = layerIds;
}

void RenderManager::SetGroupVisible(std::string_view groupName, bool visible) {
    auto it = m_layerGroups.find(std::string(groupName));
    if (it == m_layerGroups.end())
        return;

    for (int layerId : it->second) {
        auto layerIt = m_layers.find(layerId);
        if (layerIt != m_layers.end()) {
            layerIt->second.SetVisible(visible);
        }
    }
}

void RenderManager::SetGroupOpacity(std::string_view groupName, float opacity) {
    auto it = m_layerGroups.find(std::string(groupName));
    if (it == m_layerGroups.end())
        return;

    for (int layerId : it->second) {
        auto layerIt = m_layers.find(layerId);
        if (layerIt != m_layers.end()) {
            layerIt->second.SetOpacity(opacity);
        }
    }
}

void RenderManager::RenderAll(Renderer &renderer) {
    for (auto &[id, layer] : m_layers) {
        if (layer.IsVisible()) {
            layer.Render(renderer);
        }
    }
}

void RenderManager::RenderLayer(int layerId, Renderer &renderer) {
    auto it = m_layers.find(layerId);
    if (it != m_layers.end() && it->second.IsVisible()) {
        it->second.Render(renderer);
    }
}

void RenderManager::RenderGroup(std::string_view groupName,
                                Renderer &renderer) {
    auto it = m_layerGroups.find(std::string(groupName));
    if (it == m_layerGroups.end())
        return;

    for (int layerId : it->second) {
        RenderLayer(layerId, renderer);
    }
}

void RenderManager::Clear() {
    m_layers.clear();
    m_layerGroups.clear();
}

void RenderManager::ClearLayer(int layerId) {
    auto it = m_layers.find(layerId);
    if (it != m_layers.end()) {
        it->second.Clear();
    }
}
} // namespace Engine

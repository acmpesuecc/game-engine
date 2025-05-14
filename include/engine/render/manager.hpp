#ifndef _RENDER_MANAGER_HPP
#define _RENDER_MANAGER_HPP
#include <engine/render/layer.hpp>
#include <map>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
namespace Engine {
namespace Layers {
constexpr int BACKGROUND = 0;
constexpr int WORLD = 100;
constexpr int ENTITIES = 200;
constexpr int FOREGROUND = 300;
constexpr int UI = 400;
constexpr int DEBUG = 1000;
constexpr int USER_DEFINED_START = 10000;
} // namespace Layers
class RenderManager {
  public:
    RenderManager() = default;
    ~RenderManager();

    void AddRenderable(std::unique_ptr<Renderable> renderable,
                       int layerId = Layers::WORLD);
    bool RemoveRenderable(std::string_view name);
    Renderable *GetRenderable(std::string_view name);
    Renderable *GetRenderableInLayer(std::string_view name, int layerId);
    std::vector<Renderable *> GetRenderablesInLayer(int layerId);

    Layer &GetLayer(int layerId);
    bool HasLayer(int layerId) const;

    int CreateCustomLayer(int layerInFront = -1);
    int CreateCustomLayer(std::string_view layerName, int layerInFront = -1);
    void RegisterLayerName(int layerId, std::string_view layerName);
    std::string_view GetLayerName(int layerId);

    void CreateLayerGroup(std::string_view groupName,
                          const std::vector<int> &layerIds);
    void SetGroupVisible(std::string_view groupName, bool visible);
    void SetGroupOpacity(std::string_view groupName, float opacity);

    void RenderAll(Renderer &renderer);
    void RenderLayer(int layerId, Renderer &renderer);
    void RenderGroup(std::string_view groupName, Renderer &renderer);

    void Clear();
    void ClearLayer(int layerId);

  private:
    std::map<int, Layer> m_layers;
    std::unordered_map<std::string, std::vector<int>> m_layerGroups;
    static std::unordered_map<int, std::string> s_layerNames;
    static int s_nextCustomLayerId;
};
} // namespace Engine
#endif

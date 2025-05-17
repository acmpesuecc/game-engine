#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include <engine/core/renderer.hpp>
#include <engine/core/texture.hpp>
#include <memory>
#include <string_view>
#include <unordered_map>
namespace Engine {
// add more resource types later (font, audio)
enum class ResourceType { Texture };
class ResourceManager {
  public:
    explicit ResourceManager(Renderer &renderer);
    ~ResourceManager();

    void AddResource(const std::string &path, ResourceType type);
    bool RemoveResource(const std::string_view path, ResourceType type);

    std::shared_ptr<Texture> FindTexture(const std::string_view path);

  private:
    void CreateTexture(const std::string_view path);

    Renderer &m_renderer;
    std::unordered_map<std::string, ResourceType> m_resourcePaths;
    std::unordered_map<std::string_view, std::shared_ptr<Texture>> m_textureMap;
};
} // namespace Engine
#endif

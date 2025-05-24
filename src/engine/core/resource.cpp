#include <SDL3_image/SDL_image.h>
#include <engine/core/resource.hpp>
#include <engine/core/texture.hpp>
#include <memory>
#include <string_view>
namespace Engine {

ResourceManager::ResourceManager(Renderer &renderer) : m_renderer(renderer) {}

ResourceManager::~ResourceManager() {
    m_resourcePaths.clear();
    m_textureMap.clear();
}

void ResourceManager::AddResource(const std::string &path, ResourceType type) {
    m_resourcePaths.emplace(path, type);
}

bool ResourceManager::RemoveResource(const std::string_view path,
                                     ResourceType type) {
    auto it_resPath = m_resourcePaths.find(std::string(path));
    if (it_resPath == m_resourcePaths.end() || it_resPath->second != type) {
        return false;
    }
    m_resourcePaths.erase(it_resPath);
    switch (type) {
    case ResourceType::Texture: {
        auto it_texcache = m_textureMap.find(path);
        if (it_texcache != m_textureMap.end()) {
            m_textureMap.erase(it_texcache);
        }
        return true;
    }
    default:
        return false;
    }
}

std::shared_ptr<Texture>
ResourceManager::FindTexture(const std::string_view path) {
    auto it_tex = m_textureMap.find(path);
    auto it_res = m_resourcePaths.find(std::string(path));
    // if texture not loaded, and imported as asset and actually a texture
    if (it_tex == m_textureMap.end() && it_res != m_resourcePaths.end() &&
        it_res->second == ResourceType::Texture) {
        CreateTexture(path);
        return FindTexture(path);
    } else if (it_tex == m_textureMap.end()) {
        return nullptr; // else if resource not loaded but not (a texture or
                        // imported as asset)
    }
    return it_tex->second;
}

void ResourceManager::CreateTexture(const std::string_view path) {
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    SDL_Texture *sdlTex =
        IMG_LoadTexture(m_renderer.GetSDLRenderer(), std::string(path).c_str());
    if (sdlTex == nullptr) {
        // TODO: add logging here
        return;
    }
    texture->SetTexture(sdlTex);
    m_textureMap.emplace(path, std::move(texture));
}

} // namespace Engine

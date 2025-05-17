#include "engine/core/resource.hpp"
#include <engine/core/event.hpp>
#include <engine/core/renderer.hpp>
#include <engine/core/window.hpp>
#include <engine/engine.hpp>
#include <memory>

namespace Engine {
Engine &Engine::Instance() {
    static Engine instance;
    return instance;
}

Engine::~Engine() { Shutdown(); }

bool Engine::Init() {
    if (m_initialized) {
        return true;
    }
    m_window = std::make_unique<Window>();
    m_renderer = std::make_unique<Renderer>();
    if (!m_window->Init()) {
        return false;
    }
    if (!m_renderer->Init(*m_window)) {
        return false;
    }
    // TODO: Impl other subsystems here
    m_eventHandler = std::make_unique<EventManager>();
    m_renderManager = std::make_unique<RenderManager>();
    m_resManager = std::make_unique<ResourceManager>(*m_renderer);
    return true;
}

void Engine::Shutdown() {
    m_renderer->Shutdown();
    m_window->Shutdown();
    m_eventHandler->Shutdown();
    m_initialized = false;
}

Window &Engine::GetWindow() { return *m_window; }

Renderer &Engine::GetRenderer() { return *m_renderer; }

EventManager &Engine::GetEvents() { return *m_eventHandler; }

RenderManager &Engine::GetRenderManager() { return *m_renderManager; }

ResourceManager &Engine::GetResources() { return *m_resManager; };
} // namespace Engine

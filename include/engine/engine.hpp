#ifndef _ENGINE_HPP
#define _ENGINE_HPP
#include <engine/core/event.hpp>
#include <engine/core/renderer.hpp>
#include <engine/core/window.hpp>
#include <engine/render/manager.hpp>
#include <engine/render/renderable.hpp>
#include <memory>
namespace Engine {
class Window;
class Renderer;
class EventManager;
class RenderManager;
// TODO: Later implementation
// class InputHandler;
// class AudioSystem;
// class ResourceManager;
// class Time;
class Engine {
  public:
    static Engine &Instance();
    bool Init();
    void Shutdown();

    Window &GetWindow();
    Renderer &GetRenderer();
    EventManager &GetEvents();
    RenderManager &GetRenderManager();
    // InputHandler &GetInputs();
    // AudioSystem &GetAudio();
    // ResourceManager &GetResources();
    // Time &GetTime();
  private:
    Engine() = default;
    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<EventManager> m_eventHandler;
    std::unique_ptr<RenderManager> m_renderManager;
    // from main.cpp here as well

    // TODO: Later implementation
    // std::unique_ptr<InputHandler> m_inputHandler;
    // std::unique_ptr<AudioSystem> m_audioSystem;
    // std::unique_ptr<ResourceManager> m_resManager;
    // std::unique_ptr<Time> m_time;

    bool m_initialized = false;
};
} // namespace Engine
#endif

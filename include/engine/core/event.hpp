#ifndef _EVENT_HPP
#define _EVENT_HPP
#include <SDL3/SDL_events.h>
#include <functional>
#include <unordered_map>
namespace Engine {
enum class EventType {
    WindowResize,
    KeyDown,
    KeyUp,
    MouseButtonDown,
    MouseWheel,
    MouseButtonUp,
    MouseMove,
};
struct EventData {
    struct Window {
        int width;
        int height;
    };

    struct Keyboard {
        SDL_Keycode keycode;
        SDL_Scancode scancode;
        SDL_Keymod modifiers;
    };

    struct Mouse {
        float x;
        float y;
        int button;
    };

    union {
        Window window;
        Keyboard keyboard;
        Mouse mouse;
    };
};
using EventCallback = std::function<void(const EventData &)>;
class EventManager {
  public:
    EventManager() = default;
    ~EventManager();

    void Shutdown();
    bool ProcessEvent(SDL_Event *event);

    void RegisterCallback(EventType eventType, EventCallback callback);
    bool DeregisterCallback(EventType eventType);

  private:
    std::unordered_map<EventType, std::vector<EventCallback>> m_eventBindings;
    void InvokeCallback(EventType eventType, const EventData &eventData);
};
} // namespace Engine
#endif

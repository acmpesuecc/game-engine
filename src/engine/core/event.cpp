#include <engine/core/event.hpp>

namespace Engine {
EventManager::~EventManager() { Shutdown(); }

void EventManager::Shutdown() { m_eventBindings.clear(); }

void EventManager::RegisterCallback(EventType eventType,
                                    EventCallback callback) {
    if (m_eventBindings.find(eventType) == m_eventBindings.end()) {
        m_eventBindings[eventType] = std::vector<EventCallback>();
    }
    m_eventBindings[eventType].push_back(callback);
}

bool EventManager::DeregisterCallback(EventType eventType) {
    if (m_eventBindings.find(eventType) == m_eventBindings.end()) {
        return false;
    }
    m_eventBindings.erase(eventType);
    return true;
}

bool EventManager::ProcessEvent(SDL_Event *event) {
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED: {
        EventData data;
        data.window.width = event->window.data1;
        data.window.height = event->window.data2;
        InvokeCallback(EventType::WindowResize, data);
        return true;
    }

    case SDL_EVENT_KEY_DOWN: {
        EventData data;
        data.keyboard.keycode = event->key.key;
        data.keyboard.scancode = event->key.scancode;
        data.keyboard.modifiers = event->key.mod;
        InvokeCallback(EventType::KeyDown, data);
        return true;
    }

    case SDL_EVENT_KEY_UP: {
        EventData data;
        data.keyboard.keycode = event->key.key;
        data.keyboard.scancode = event->key.scancode;
        data.keyboard.modifiers = event->key.mod;
        InvokeCallback(EventType::KeyUp, data);
        return true;
    }

    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        EventData data;
        data.mouse.x = event->button.x;
        data.mouse.y = event->button.y;
        data.mouse.button = event->button.button;
        InvokeCallback(EventType::MouseButtonDown, data);
        return true;
    }

    case SDL_EVENT_MOUSE_BUTTON_UP: {
        EventData data;
        data.mouse.x = event->button.x;
        data.mouse.y = event->button.y;
        data.mouse.button = event->button.button;
        InvokeCallback(EventType::MouseButtonUp, data);
        return true;
    }

    case SDL_EVENT_MOUSE_MOTION: {
        EventData data;
        data.mouse.x = event->motion.x;
        data.mouse.y = event->motion.y;
        InvokeCallback(EventType::MouseMove, data);
        return true;
    }

    case SDL_EVENT_MOUSE_WHEEL: {
        EventData data;
        data.mouse.x = event->wheel.x;
        data.mouse.y = event->wheel.y;
        InvokeCallback(EventType::MouseWheel, data);
        return true;
    }
    }
    return false;
}

void EventManager::InvokeCallback(EventType type, const EventData &data) {
    auto it = m_eventBindings.find(type);
    if (it != m_eventBindings.end()) {
        for (std::function<void(const EventData &)> callback : it->second) {
            callback(data);
        }
    }
}
} // namespace Engine

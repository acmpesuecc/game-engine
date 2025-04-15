#include <engine/core/window.hpp>

namespace Engine {
Window::~Window() { Shutdown(); }

bool Window::Init(const WindowProps &props) {
    m_title = props.title;
    m_width = props.width;
    m_height = props.height;
    m_properties = SDL_CreateProperties();
    SDL_SetBooleanProperty(m_properties,
                           SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN,
                           props.resizable);
    SDL_SetBooleanProperty(m_properties,
                           SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN,
                           props.fullscreen);
    SDL_SetBooleanProperty(m_properties,
                           SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN,
                           props.hdpi);
    SDL_SetStringProperty(m_properties, SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                          m_title.c_str());
    SDL_SetNumberProperty(m_properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER,
                          m_width);
    SDL_SetNumberProperty(m_properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER,
                          m_height);
    m_window = SDL_CreateWindowWithProperties(m_properties);
    return m_window != nullptr;
}

void Window::Shutdown() {
    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

void Window::SetTitle(const std::string &title) {
    m_title = title;
    SDL_SetWindowTitle(m_window, m_title.c_str());
}

void Window::SetDimensions(uint16_t width, uint16_t height) {
    m_width = width;
    m_height = height;
    SDL_SetWindowSize(m_window, width, height);
}

void Window::SetFullscreen(bool fullscreen) {
    SDL_SetWindowFullscreen(m_window, fullscreen);
}
} // namespace Engine

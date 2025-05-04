#ifndef _WINDOW_HPP
#define _WINDOW_HPP
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <string>
namespace Engine {
struct WindowProps {
    std::string title;
    uint16_t width = 1920;
    uint16_t height = 1080;
    bool resizable = true;
    bool fullscreen = false;
    bool hdpi = false;
};
class Window {
  public:
    Window() = default;
    ~Window();

    bool Init(const WindowProps &props = WindowProps());
    void Shutdown();

    void SetTitle(const std::string &title);
    void SetDimensions(uint16_t width, uint16_t height);
    void SetFullscreen(bool fullscreen);

    SDL_Window *GetSDLWindow() const { return m_window; }
    uint16_t GetWidth() const { return m_width; }
    uint16_t GetHeight() const { return m_height; }

  private:
    SDL_Window *m_window = nullptr;
    SDL_PropertiesID m_properties = 0;
    uint16_t m_width = 0;
    uint16_t m_height = 0;
    std::string m_title;
};
} // namespace Engine
#endif

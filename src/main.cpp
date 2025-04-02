#include <SDL3/SDL.h>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <iostream>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int main() {
    bool init{SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)};
    if (!init) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window *mainWindow{
        SDL_CreateWindow("SDL3 Window", SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT)};
    if (mainWindow == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Renderer *mainRenderer{SDL_CreateRenderer(mainWindow, 0)};
    if (mainRenderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
        return -1;
    }
    SDL_Texture *mainTexture{SDL_CreateTexture(
        mainRenderer, SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT)};
    if (mainTexture == nullptr) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(mainRenderer);
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
        return -1;
    }
    SDL_Event event;
    bool quit{false};
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_TERMINATING) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 128);
        SDL_RenderClear(mainRenderer);
        SDL_RenderPresent(mainRenderer);
    }
    SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    return 0;
}

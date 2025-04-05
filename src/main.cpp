#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int COLOR_RED = 255;
const int COLOR_GREEN = 0;
const int COLOR_BLUE = 0;
const int COLOR_ALPHA = 128;

int main() {
    bool init{SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)};
    if (!init) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n",
                        SDL_GetError());
        return -1;
    }
    SDL_Window *mainWindow{
        SDL_CreateWindow("SDL3 Window", SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT)};
    if (mainWindow == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s\n",
                        SDL_GetError());
        return -1;
    }
    SDL_Renderer *mainRenderer{SDL_CreateRenderer(mainWindow, NULL)};
    if (mainRenderer == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
        return -1;
    }
    /* Currently unused
    SDL_Texture *mainTexture{SDL_CreateTexture(
        mainRenderer, SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STATIC,
        SCREEN_WIDTH, SCREEN_HEIGHT)};
    if (mainTexture == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "SDL_CreateTexture Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(mainRenderer);
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
        return -1;
    } */
    SDL_Event event;
    bool quit{false};
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_TERMINATING) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(mainRenderer, COLOR_RED, COLOR_GREEN, COLOR_BLUE,
                               COLOR_ALPHA);
        SDL_RenderClear(mainRenderer);
        SDL_RenderPresent(mainRenderer);
    }
    // SDL_DestroyTexture(mainTexture);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    return 0;
}

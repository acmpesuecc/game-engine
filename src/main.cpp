#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int COLOR_RED = 255;
const int COLOR_GREEN = 0;
const int COLOR_BLUE = 0;
const int COLOR_ALPHA = 128;

struct AppState {
    SDL_Window *mainWindow;
    SDL_Renderer *mainRenderer;
    // Textures and other subsystems can be added at a later stage
    SDL_AppResult appResult = SDL_APP_CONTINUE;
};

// A simple helper function that logs that a critical failure happens and
// returns an app failure.
SDL_AppResult SDL_Failure(const char *fmt) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s: %s\n", fmt, SDL_GetError());
    return SDL_APP_FAILURE;
}

// Initialises subsystems and initialises appState to be used by all other main functions.
SDL_AppResult SDL_AppInit(void **appState, int argc, char *argv[]) {
    AppState *state{new AppState{
        .mainWindow = NULL,
        .mainRenderer =
            NULL}}; // This only is useful this way if we are initialising
                    // subsystems using pointers, which isn't very common. We
                    // can instead initialise the state after all subsystems are
                    // initialised, but this depends on which subsystems we are
                    // initialising.
    SDL_SetAppMetadata("GameEngine", "0.0.1",
                       "org.acm.pesuecc.aiep.game-engine");
    bool init{SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)};
    if (!init) {
        return SDL_Failure("Error initialising SDL3");
    }
    init = SDL_CreateWindowAndRenderer(
        "GameEngine", SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT, &(state->mainWindow),
        &(state->mainRenderer));
    if (!init) {
        return SDL_Failure("Error creating Window and Renderer");
    }
    *appState = state;
    return SDL_APP_CONTINUE;
}

// The event handling function called by SDL.
// TODO: Try and make a genericsed function that is called from here to allow
//       custom event handling to be added by a user.
SDL_AppResult SDL_AppEvent(void *appState, SDL_Event *event) {
    AppState *state = static_cast<AppState *>(appState);
    if (event->type == SDL_EVENT_QUIT || event->type == SDL_EVENT_TERMINATING) {
        state->appResult = SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// The "main loop" of the window.
SDL_AppResult SDL_AppIterate(void *appState) {
    AppState *state = static_cast<AppState *>(appState);
    SDL_SetRenderDrawColor(state->mainRenderer, COLOR_RED, COLOR_GREEN,
                           COLOR_BLUE, COLOR_ALPHA);
    SDL_RenderClear(state->mainRenderer);
    SDL_RenderPresent(state->mainRenderer);
    return state->appResult;
}

// Cleans up the initialised subsystems.
void SDL_AppQuit(void *appState, SDL_AppResult result) {
    AppState *state = static_cast<AppState *>(appState);
    if (state != nullptr) {
        SDL_DestroyRenderer(state->mainRenderer);
        SDL_DestroyWindow(state->mainWindow);
        delete state;
    }
}

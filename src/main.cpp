#include <iostream>
#include <ctime>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_keyboard.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int COLOR_RED = 255;
const int COLOR_GREEN = 0;
const int COLOR_BLUE = 0;
const int COLOR_ALPHA = 128;

struct AppState {
    SDL_Window *mainWindow;
    SDL_Renderer *mainRenderer;
    SDL_Rect Canvas = { 0,0,0,0 };
    SDL_Surface* tile_map;
    SDL_Texture* tile_map_texture;
    int windowWidth = SCREEN_WIDTH;
    int windowHeight = SCREEN_HEIGHT;
	int offSetX = 0;
	int offSetY = 0; 
	float zoom = 1.0f; // Zoom factor for the canvas
	int tilemap[100][100] = { 0 };
	bool tilemapInitialised = false; // Just to check if the tilemap has been initialised 
    // Textures and other subsystems can be added at a later stage
    SDL_AppResult appResult = SDL_APP_CONTINUE;
};

// A simple helper function that logs that a critical failure happens and
// returns an app failure.
SDL_AppResult SDL_Failure(const char *fmt) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s: %s\n", fmt, SDL_GetError());
    return SDL_APP_FAILURE;
}

// Initialises subsystems and initialises appState to be used by all other main
// functions.
SDL_AppResult SDL_AppInit(void** appState, int argc, char* argv[]) {
    srand((unsigned int)time(NULL)); 
    AppState* state = new AppState();
    state->mainWindow = nullptr;
    state->mainRenderer = nullptr; // This only is useful this way if we are initialising
                                   // subsystems using pointers, which isn't very common. We
                                   // can instead initialise the state after all subsystems
                                   // are initialised, but this depends on which subsystems
                                   // we are initialising.
    state->tile_map = nullptr;
    state->tile_map_texture = nullptr;
    SDL_SetAppMetadata("GameEngine", "0.0.1",
        "org.acm.pesuecc.aiep.game-engine");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        return SDL_Failure("Error initialising SDL3");
    }
    bool initWindowRenderer{SDL_CreateWindowAndRenderer(
        "GameEngine", SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT, &(state->mainWindow),
        &(state->mainRenderer))};
    if (!initWindowRenderer) {
        return SDL_Failure("Error creating Window and Renderer");
    }

	// Loading tilemap after creating the window and renderer
    state->tile_map = SDL_LoadBMP("./src/white_tile_32by32.bmp");
    if (!state->tile_map) {
        return SDL_Failure("Failed to load BMP for tilemap");
    }
    state->tile_map_texture = SDL_CreateTextureFromSurface(state->mainRenderer, state->tile_map);
    if (!state->tile_map_texture) {
        return SDL_Failure("Failed to create texture from surface");
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
    else if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        // keeping window size in sync when resized
        state->windowWidth = event->window.data1;
        state->windowHeight = event->window.data2;
        SDL_Log("Window resized to %d x %d", state->windowWidth,
            state->windowHeight);
    }
    else if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Log("Key pressed: %c", event->key.key);
    }
    else if (event->type == SDL_EVENT_KEY_UP) {
        SDL_Log("Key was released: %c", event->key.key);
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_Log("Mouse button pressed");
    }
    else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        SDL_Log("Mouse wheel scrolled: %.0f", event->wheel.y);
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        SDL_Log("Mouse button released");
    }
    else if (event->type == SDL_EVENT_MOUSE_MOTION) {
        SDL_Log("Mouse moved to: (%.0f, %.0f)", event->motion.x, event->motion.y);
    }
    return SDL_APP_CONTINUE;
}

static void RenderCanvas(AppState* state) { // So everything that has to do with the canvas; the tilemap,
    //  the sprites, etc. are to be included under this function
// Rendering the Top left corner viewport 
    state->Canvas.x = 300;
    state->Canvas.y = 0;
    state->Canvas.w = state->windowWidth / 1.5625;
    state->Canvas.h = state->windowHeight / 1.6667;

    // Rendering the "partially" top left viewport 
    SDL_SetRenderViewport(state->mainRenderer, &(state->Canvas));
    SDL_SetRenderDrawColor(state->mainRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(state->mainRenderer, NULL);

    // Rendering the tilemap
    if (!state->tilemapInitialised) {
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                state->tilemap[i][j] = rand() % 4 + 1;
            }
        }
        state->tilemapInitialised = true;
    }

    //Populating the screen with tiles
    SDL_FRect tile[100][100];
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            tile[i][j].x = (i * 32) - state->offSetX;
            tile[i][j].y = (j * 32) - state->offSetY;
            tile[i][j].w = 32.0f * state->zoom;
            tile[i][j].h = 32.0f * state->zoom;
        }
    }

    // Selecting the tile to be rendered
    SDL_FRect select_tile_1;
    select_tile_1.x = 0;
    select_tile_1.y = 0;
    select_tile_1.w = 32;
    select_tile_1.h = 32;
    SDL_FRect select_tile_2; 
    select_tile_2.x = 32;
    select_tile_2.y = 0;
    select_tile_2.w = 32;
    select_tile_2.h = 32;
    SDL_FRect select_tile_3;
    select_tile_3.x = 0;
    select_tile_3.y = 32;
    select_tile_3.w = 32;
    select_tile_3.h = 32;
    SDL_FRect select_tile_4;
    select_tile_4.x = 32;
    select_tile_4.y = 32;
    select_tile_4.w = 32;
    select_tile_4.h = 32;

    // Rendering the tilemap
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (state->tilemap[i][j] == 1) {
                SDL_RenderTexture(state->mainRenderer, state->tile_map_texture, &select_tile_1, &tile[i][j]);
            }
            else if (state->tilemap[i][j] == 2) {
                SDL_RenderTexture(state->mainRenderer, state->tile_map_texture, &select_tile_2, &tile[i][j]);
            }
            else if (state->tilemap[i][j] == 3) {
                SDL_RenderTexture(state->mainRenderer, state->tile_map_texture, &select_tile_3, &tile[i][j]);
            }
            else if (state->tilemap[i][j] == 4) {
                SDL_RenderTexture(state->mainRenderer, state->tile_map_texture, &select_tile_4, &tile[i][j]);
            }
        }
    }
}

// The "main loop" of the window.
SDL_AppResult SDL_AppIterate(void *appState) {
    AppState* state = static_cast<AppState *>(appState);

    //Handling keyboard inputs
    const bool* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) {
        state->offSetX -= 5;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        state->offSetX += 5;
    }
    if (keys[SDL_SCANCODE_UP]) {
        state->offSetY -= 5;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        state->offSetY += 5;
    }
    if (keys[SDL_SCANCODE_MINUS]) {
		state->zoom *= 0.95f;
        if (state->zoom < 0.05f) {
			state->zoom = 0.05f;
        }
    }
	if (keys[SDL_SCANCODE_EQUALS]) {
		state->zoom *= 1.05f;
		if (state->zoom > 5.0f) {
			state->zoom = 5.0f;
		}
	}
	if (state->offSetX < 0) {
		state->offSetX = 0;
	}
	if (state->offSetY < 0) {
		state->offSetY = 0;
	}
	if (state->offSetX > 1400) {
		state->offSetX = 1400;
	}
	if (state->offSetY > 1800) {
		state->offSetY = 1800;
	}

    // Rendering the window
    SDL_SetRenderViewport(state->mainRenderer, NULL);
    SDL_SetRenderDrawColor(state->mainRenderer, COLOR_RED, COLOR_GREEN,
        COLOR_BLUE, COLOR_ALPHA);
    SDL_RenderClear(state->mainRenderer);

	// Rendering the Canvas, and all the componenets 
    SDL_Delay(20);
	RenderCanvas(state);

    SDL_RenderPresent(state->mainRenderer);
    return state->appResult;
}

// Cleans up the initialised subsystems.
void SDL_AppQuit(void *appState, SDL_AppResult result) {
    AppState* state = static_cast<AppState *>(appState);
    if (state != nullptr) {
        SDL_DestroyTexture(state->tile_map_texture);
		SDL_DestroySurface(state->tile_map);
        SDL_DestroyRenderer(state->mainRenderer);
        SDL_DestroyWindow(state->mainWindow);
        delete state;
    }
}

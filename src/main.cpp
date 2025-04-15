#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <engine/engine.hpp>
#include <memory>

// Used in this example to keep track of selected object
static int s_currentRect = 1;

// A simple helper function that logs that a critical failure happens and
// returns an app failure.
SDL_AppResult SDL_Failure(const char *fmt) {
    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s: %s\n", fmt, SDL_GetError());
    return SDL_APP_FAILURE;
}

// Initialises subsystems and initialises appState to be used by all other main
// functions.
SDL_AppResult SDL_AppInit(void **appState, int argc, char *argv[]) {
    Engine::Engine *gameEngine = &Engine::Engine::Instance();
    SDL_SetAppMetadata("GameEngine", "0.0.1",
                       "org.acm.pesuecc.aiep.game-engine");
    if (!gameEngine->Init()) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
                        "Failed to initialise game engine");
        gameEngine->Shutdown();
        return SDL_APP_FAILURE;
    }
    gameEngine->GetWindow().SetDimensions(1000, 800);
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::WindowResize, [gameEngine](Engine::EventData data) {
            SDL_Log("Window resized to %d x %d", data.window.width,
                    data.window.height);
            gameEngine->GetWindow().SetDimensions(data.window.width,
                                                  data.window.height);
        });
    /*
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::KeyDown, [](Engine::EventData data) {
            SDL_Log("Key pressed: %u (keycode)", data.keyboard.keycode);
            SDL_Log("Key pressed: %u (scancode)", data.keyboard.scancode);
            SDL_Log("Key pressed modifiers: %u", data.keyboard.modifiers);
        });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::KeyUp, [](Engine::EventData data) {
            SDL_Log("Key released: %u (keycode)", data.keyboard.keycode);
            SDL_Log("Key released: %u (scancode)", data.keyboard.scancode);
            SDL_Log("Key released modifiers: %u", data.keyboard.modifiers);
        });
    */
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseButtonDown,
        [](Engine::EventData data) { SDL_Log("Mouse buttons pressed"); });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseWheel, [](Engine::EventData data) {
            SDL_Log("Mouse wheel scrolled: %.0f", data.mouse.y);
        });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseButtonUp,
        [](Engine::EventData data) { SDL_Log("Mouse buttons released"); });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseMove, [](Engine::EventData data) {
            SDL_Log("Mouse moved to: (%.0f, %.0f)", data.mouse.x, data.mouse.y);
        });
    std::unique_ptr<Engine::Renderable> rectangle1 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(10, 10, 60, 80),
                                                 Engine::Color::Blue());
    rectangle1->SetName("Rectangle1");
    std::unique_ptr<Engine::Renderable> rectangle2 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(270, 10, 60, 80),
                                                 Engine::Color::Green());
    rectangle2->SetName("Rectangle2");
    std::unique_ptr<Engine::Renderable> rectangle3 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(10, 120, 60, 80),
                                                 Engine::Color::Red());
    rectangle3->SetName("Rectangle3");
    std::unique_ptr<Engine::Renderable> rectangle4 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(270, 120, 60, 80),
                                                 Engine::Color::White());
    rectangle4->SetName("Rectangle4");
    gameEngine->GetRenderManager().AddRenderable(std::move(rectangle1),
                                                 Engine::Layers::WORLD);
    gameEngine->GetRenderManager().AddRenderable(std::move(rectangle2),
                                                 Engine::Layers::ENTITIES);
    gameEngine->GetRenderManager().AddRenderable(std::move(rectangle3),
                                                 Engine::Layers::FOREGROUND);
    gameEngine->GetRenderManager().AddRenderable(std::move(rectangle4),
                                                 Engine::Layers::UI);
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::KeyDown, [](Engine::EventData data) {
            if (data.keyboard.keycode == SDLK_1) {
                s_currentRect = 1;
                SDL_Log("Rectangle 1 selected");
            } else if (data.keyboard.keycode == SDLK_2) {
                s_currentRect = 2;
                SDL_Log("Rectangle 2 selected");
            } else if (data.keyboard.keycode == SDLK_3) {
                s_currentRect = 3;
                SDL_Log("Rectangle 3 selected");
            } else if (data.keyboard.keycode == SDLK_4) {
                s_currentRect = 4;
                SDL_Log("Rectangle 4 selected");
            }
        });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::KeyDown, [gameEngine](Engine::EventData data) {
            Engine::Renderable *rect = nullptr;
            switch (s_currentRect) {
            case 1:
                rect =
                    gameEngine->GetRenderManager().GetRenderable("Rectangle1");
                break;
            case 2:
                rect =
                    gameEngine->GetRenderManager().GetRenderable("Rectangle2");
                break;
            case 3:
                rect =
                    gameEngine->GetRenderManager().GetRenderable("Rectangle3");
                break;
            case 4:
                rect =
                    gameEngine->GetRenderManager().GetRenderable("Rectangle4");
                break;
            }
            if (data.keyboard.keycode == SDLK_RIGHT) {
                rect->TranslateX(10.0f);
            } else if (data.keyboard.keycode == SDLK_LEFT) {
                rect->TranslateX(-10.0f);
            } else if (data.keyboard.keycode == SDLK_DOWN) {
                rect->TranslateY(10.0f);
            } else if (data.keyboard.keycode == SDLK_UP) {
                rect->TranslateY(-10.0f);
            } else if (data.keyboard.keycode == SDLK_RSHIFT) {
                rect->Rotate(10.0f);
            } else if (data.keyboard.keycode == SDLK_LSHIFT) {
                rect->Rotate(-10.0f);
            } else if (data.keyboard.keycode == SDLK_P) {
                rect->Scale(1.1f);
            } else if (data.keyboard.keycode == SDLK_Q) {
                rect->Scale(0.9f);
            } else if (data.keyboard.keycode == SDLK_SPACE) {
                auto &layer = gameEngine->GetRenderManager().GetLayer(
                    s_currentRect * 100);
                float opacity = layer.GetOpacity();
                layer.SetOpacity(opacity + 0.1f);
            } else if (data.keyboard.keycode == SDLK_LCTRL) {
                auto &layer = gameEngine->GetRenderManager().GetLayer(
                    s_currentRect * 100);
                float opacity = layer.GetOpacity();
                layer.SetOpacity(opacity - 0.1f);
            }
        });
    SDL_Log("Controls:\n1-4: Change selected object\n"
            "Arrow Keys: Move object\n"
            "RShift: Rotate Clockewise\n"
            "LShift: Rotate Counter-Clockwise\n"
            "P: Increase Scale\n"
            "Q: Decrease Scale\n"
            "Space: Increase Opacity\n"
            "Ctrl: Decrease Opacity");
    *appState = gameEngine;
    return SDL_APP_CONTINUE;
}

// The event handling function called by SDL.
// TODO: Try and make a genericsed function that is called from here to allow
//       custom event handling to be added by a user.
SDL_AppResult SDL_AppEvent(void *appState, SDL_Event *event) {
    Engine::Engine *gameEngine = static_cast<Engine::Engine *>(appState);
    if (event->type == SDL_EVENT_QUIT || event->type == SDL_EVENT_TERMINATING) {
        return SDL_APP_SUCCESS;
    } else {
        gameEngine->GetEvents().ProcessEvent(event);
    }
    return SDL_APP_CONTINUE;
}

// The "main loop" of the window.
SDL_AppResult SDL_AppIterate(void *appState) {
    Engine::Engine *gameEngine = static_cast<Engine::Engine *>(appState);
    gameEngine->GetRenderer().Clear(Engine::Color::Black());
    gameEngine->GetRenderManager().RenderAll(gameEngine->GetRenderer());
    gameEngine->GetRenderer().Present();
    return SDL_APP_CONTINUE;
}

// Cleans up the initialised subsystems.
void SDL_AppQuit(void *appState, SDL_AppResult result) {
    Engine::Engine *gameEngine = static_cast<Engine::Engine *>(appState);
    if (gameEngine != nullptr) {
        gameEngine->Shutdown();
    }
}

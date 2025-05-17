#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <engine/engine.hpp>
#include <memory>

// Used in this example to keep track of selected object
static int s_currentObj = 1;

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
    Engine::Window &gameWindow = gameEngine->GetWindow();
    Engine::EventManager &events = gameEngine->GetEvents();
    Engine::ResourceManager &res = gameEngine->GetResources();
    Engine::RenderManager &rdrMgr = gameEngine->GetRenderManager();
    gameWindow.SetDimensions(1000, 800);
    events.RegisterCallback(
        Engine::EventType::WindowResize, [&gameWindow](Engine::EventData data) {
            SDL_Log("Window resized to %d x %d", data.window.width,
                    data.window.height);
            gameWindow.SetDimensions(data.window.width, data.window.height);
        });
    events.RegisterCallback(
        Engine::EventType::MouseButtonDown,
        [](Engine::EventData data) { SDL_Log("Mouse buttons pressed"); });
    events.RegisterCallback(
        Engine::EventType::MouseWheel, [](Engine::EventData data) {
            SDL_Log("Mouse wheel scrolled: %.0f", data.mouse.y);
        });
    events.RegisterCallback(
        Engine::EventType::MouseButtonUp,
        [](Engine::EventData data) { SDL_Log("Mouse buttons released"); });
    events.RegisterCallback(
        Engine::EventType::MouseMove, [](Engine::EventData data) {
            SDL_Log("Mouse moved to: (%.0f, %.0f)", data.mouse.x, data.mouse.y);
        });
    res.AddResource("tests/res/kenney-pixel-char1.png",
                    Engine::ResourceType::Texture);
    res.AddResource("tests/res/kenney-pixel-char2.png",
                    Engine::ResourceType::Texture);
    std::shared_ptr<Engine::Texture> tex1 =
        res.FindTexture("tests/res/kenney-pixel-char1.png");
    std::unique_ptr<Engine::Renderable> obj1 = std::make_unique<Engine::Sprite>(
        std::move(tex1), Engine::Vector2(15, 25));
    obj1->SetName("Character 1");
    std::unique_ptr<Engine::Renderable> obj2 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(270, 10, 60, 80),
                                                 Engine::Color::Green());
    obj2->SetName("Green Rectangle");
    std::unique_ptr<Engine::Renderable> rectangle3 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(10, 120, 60, 80),
                                                 Engine::Color::Red());
    rectangle3->SetName("Red Rectangle");
    std::shared_ptr<Engine::Texture> tex2 =
        res.FindTexture("tests/res/kenney-pixel-char2.png");
    std::unique_ptr<Engine::Renderable> rectangle4 =
        std::make_unique<Engine::Sprite>(std::move(tex2),
                                         Engine::Vector2(300, 160));
    rectangle4->SetName("Character 2");
    rdrMgr.AddRenderable(std::move(obj1), Engine::Layers::WORLD);
    rdrMgr.AddRenderable(std::move(obj2), Engine::Layers::ENTITIES);
    rdrMgr.AddRenderable(std::move(rectangle3), Engine::Layers::FOREGROUND);
    rdrMgr.AddRenderable(std::move(rectangle4), Engine::Layers::UI);
    events.RegisterCallback(Engine::EventType::KeyDown,
                            [](Engine::EventData data) {
                                if (data.keyboard.keycode == SDLK_1) {
                                    s_currentObj = 1;
                                    SDL_Log("Character 1 selected");
                                } else if (data.keyboard.keycode == SDLK_2) {
                                    s_currentObj = 2;
                                    SDL_Log("Green Rectangle selected");
                                } else if (data.keyboard.keycode == SDLK_3) {
                                    s_currentObj = 3;
                                    SDL_Log("Red Rectangle selected");
                                } else if (data.keyboard.keycode == SDLK_4) {
                                    s_currentObj = 4;
                                    SDL_Log("Character 2 selected");
                                }
                            });
    events.RegisterCallback(
        Engine::EventType::KeyDown, [&rdrMgr](Engine::EventData data) {
            Engine::Renderable *obj = nullptr;
            switch (s_currentObj) {
            case 1:
                obj = rdrMgr.GetRenderable("Character 1");
                break;
            case 2:
                obj = rdrMgr.GetRenderable("Green Rectangle");
                break;
            case 3:
                obj = rdrMgr.GetRenderable("Red Rectangle");
                break;
            case 4:
                obj = rdrMgr.GetRenderable("Character 2");
                break;
            }
            if (data.keyboard.keycode == SDLK_RIGHT) {
                obj->TranslateX(10.0f);
            } else if (data.keyboard.keycode == SDLK_LEFT) {
                obj->TranslateX(-10.0f);
            } else if (data.keyboard.keycode == SDLK_DOWN) {
                obj->TranslateY(10.0f);
            } else if (data.keyboard.keycode == SDLK_UP) {
                obj->TranslateY(-10.0f);
            } else if (data.keyboard.keycode == SDLK_RSHIFT) {
                obj->Rotate(10.0f);
            } else if (data.keyboard.keycode == SDLK_LSHIFT) {
                obj->Rotate(-10.0f);
            } else if (data.keyboard.keycode == SDLK_P) {
                obj->Scale(1.1f);
            } else if (data.keyboard.keycode == SDLK_Q) {
                obj->Scale(0.9f);
            } else if (data.keyboard.keycode == SDLK_SPACE) {
                auto &layer = rdrMgr.GetLayer(s_currentObj * 100);
                float opacity = layer.GetOpacity();
                layer.SetOpacity(opacity + 0.1f);
            } else if (data.keyboard.keycode == SDLK_LCTRL) {
                auto &layer = rdrMgr.GetLayer(s_currentObj * 100);
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

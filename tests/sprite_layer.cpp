#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <engine/engine.hpp>
#include <engine/logger.hpp>
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
    SPDLOG_INFO("Initializing application.");
    Engine::Logging::Init();

    Engine::Engine *gameEngine = &Engine::Engine::Instance();
    SDL_SetAppMetadata("GameEngine", "0.0.1",
                       "org.acm.pesuecc.aiep.game-engine");
    if (!gameEngine->Init()) {
        SPDLOG_CRITICAL("Failed to initialize game engine.");
        gameEngine->Shutdown();
        return SDL_APP_FAILURE;
    }
    SPDLOG_INFO("Game engine initialized successfully.");

    Engine::Window &gameWindow = gameEngine->GetWindow();
    Engine::EventManager &events = gameEngine->GetEvents();
    Engine::ResourceManager &res = gameEngine->GetResources();
    Engine::RenderManager &rdrMgr = gameEngine->GetRenderManager();
    gameWindow.SetDimensions(1000, 800);
    events.RegisterCallback(
        Engine::EventType::WindowResize, [&gameWindow](Engine::EventData data) {
            SPDLOG_INFO("Window resized to {} x {}", data.window.width,
                        data.window.height);
            gameWindow.SetDimensions(data.window.width, data.window.height);
        });
    events.RegisterCallback(
        Engine::EventType::MouseButtonDown,
        [](Engine::EventData data) { SPDLOG_INFO("Mouse buttons pressed"); });
    events.RegisterCallback(
        Engine::EventType::MouseWheel, [](Engine::EventData data) {
            SPDLOG_INFO("Mouse wheel scrolled: {:.0f}", data.mouse.y);
        });
    events.RegisterCallback(
        Engine::EventType::MouseButtonUp,
        [](Engine::EventData data) { SPDLOG_INFO("Mouse buttons released."); });
    events.RegisterCallback(
        Engine::EventType::MouseMove, [](Engine::EventData data) {
            SPDLOG_INFO("Mouse moved to: ({:.0f}, {:.0f})", data.mouse.x,
                        data.mouse.y);
        });

    SPDLOG_INFO("Creating renderable objects.");

    SPDLOG_INFO("Importing textures.");
    res.AddResource("tests/res/kenney-pixel-char1.png",
                    Engine::ResourceType::Texture);
    res.AddResource("tests/res/kenney-pixel-char2.png",
                    Engine::ResourceType::Texture);

    SPDLOG_INFO("Loading textures and creating renderable objects.");
    std::shared_ptr<Engine::Texture> tex1 =
        res.FindTexture("tests/res/kenney-pixel-char1.png");
    std::unique_ptr<Engine::Renderable> obj1 = std::make_unique<Engine::Sprite>(
        std::move(tex1), Engine::Vector2(15, 25));
    obj1->SetName("Character 1");
    std::unique_ptr<Engine::Renderable> obj2 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(270, 10, 60, 80),
                                                 Engine::Color::Green());
    obj2->SetName("Green Rectangle");
    std::unique_ptr<Engine::Renderable> obj3 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(10, 120, 60, 80),
                                                 Engine::Color::Red());
    obj3->SetName("Red Rectangle");
    std::shared_ptr<Engine::Texture> tex2 =
        res.FindTexture("tests/res/kenney-pixel-char2.png");
    std::unique_ptr<Engine::Renderable> obj4 =
        std::make_unique<Engine::Sprite>(std::move(tex2),
                                         Engine::Vector2(300, 160));
    obj4->SetName("Character 2");

    SPDLOG_INFO("Adding renderable objects to render manager.");
    rdrMgr.AddRenderable(std::move(obj1), Engine::Layers::WORLD);
    rdrMgr.AddRenderable(std::move(obj2), Engine::Layers::ENTITIES);
    rdrMgr.AddRenderable(std::move(rectangle3), Engine::Layers::FOREGROUND);
    rdrMgr.AddRenderable(std::move(rectangle4), Engine::Layers::UI);

    events.RegisterCallback(Engine::EventType::KeyDown,
                            [](Engine::EventData data) {
                                if (data.keyboard.keycode == SDLK_1) {
                                    s_currentObj = 1;
                                    SPDLOG_INFO("Character 1 selected");
                                } else if (data.keyboard.keycode == SDLK_2) {
                                    s_currentObj = 2;
                                    SPDLOG_INFO("Green Rectangle selected");
                                } else if (data.keyboard.keycode == SDLK_3) {
                                    s_currentObj = 3;
                                    SPDLOG_INFO("Red Rectangle selected");
                                } else if (data.keyboard.keycode == SDLK_4) {
                                    s_currentObj = 4;
                                    SPDLOG_INFO("Character 2 selected");
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
    SPDLOG_INFO("Controls:\n1-4: Change selected object\n"
            "Arrow Keys: Move object\n"
            "RShift: Rotate Clockewise\n"
            "LShift: Rotate Counter-Clockwise\n"
            "P: Increase Scale\n"
            "Q: Decrease Scale\n"
            "Space: Increase Opacity\n"
            "Ctrl: Decrease Opacity");

    SPDLOG_INFO("Application initialized successfully.");
    *appState = gameEngine;
    return SDL_APP_CONTINUE;
}

// The event handling function called by SDL.
// TODO: Try and make a genericsed function that is called from here to allow
//       custom event handling to be added by a user.
SDL_AppResult SDL_AppEvent(void *appState, SDL_Event *event) {
    Engine::Engine *gameEngine = static_cast<Engine::Engine *>(appState);
    if (event->type == SDL_EVENT_QUIT || event->type == SDL_EVENT_TERMINATING) {
        SPDLOG_INFO("Received SDL_EVENT_QUIT or SDL_EVENT_TERMINATING. Exiting "
                    "application.");
        return SDL_APP_SUCCESS;
    } else {
        SPDLOG_DEBUG("Processing SDL event of type: {}", event->type);
        gameEngine->GetEvents().ProcessEvent(event);
    }
    return SDL_APP_CONTINUE;
}

// The "main loop" of the window.
SDL_AppResult SDL_AppIterate(void *appState) {
    Engine::Engine *gameEngine = static_cast<Engine::Engine *>(appState);
    SPDLOG_TRACE("Starting main loop iteration.");

    SPDLOG_DEBUG("Clearing renderer with color: Black.");
    gameEngine->GetRenderer().Clear(Engine::Color::Black());

    SPDLOG_DEBUG("Rendering all objects.");
    gameEngine->GetRenderManager().RenderAll(gameEngine->GetRenderer());

    SPDLOG_DEBUG("Presenting rendered frame.");
    gameEngine->GetRenderer().Present();

    SPDLOG_TRACE("Main loop iteration completed.");
    return SDL_APP_CONTINUE;
}

// Cleans up the initialised subsystems.
void SDL_AppQuit(void *appState, SDL_AppResult result) {
    Engine::Engine *gameEngine = static_cast<Engine::Engine *>(appState);
    if (gameEngine != nullptr) {
        SPDLOG_INFO("Shutting down game engine.");
        gameEngine->Shutdown();
        SPDLOG_INFO("Game engine shutdown completed.");
    } else {
        SPDLOG_WARN("Game engine was null during shutdown.");
    }
}

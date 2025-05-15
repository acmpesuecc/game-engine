#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <engine/engine.hpp>
#include <engine/logger.hpp>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

// Used in this example to keep track of selected object
static int s_currentRect = 1;

// Initialises subsystems and initialises appState to be used by all other main
// functions.
SDL_AppResult SDL_AppInit(void **appState, int argc, char *argv[]) {
    SPDLOG_INFO("Initializing application.");
    Engine::Logging::Init();

    Engine::Engine *gameEngine = &Engine::Engine::Instance();
    SDL_SetAppMetadata("GameEngine", "0.0.1",
                       "org.acm.pesuecc.aiep.game-engine");

    SPDLOG_INFO("Initializing game engine.");
    if (!gameEngine->Init()) {
        SPDLOG_CRITICAL("Failed to initialize game engine.");
        return SDL_APP_FAILURE;
    }
    SPDLOG_INFO("Game engine initialized successfully.");

    SPDLOG_INFO("Setting window dimensions to 1000x800.");
    gameEngine->GetWindow().SetDimensions(1000, 800);

    SPDLOG_INFO("Registering event callbacks.");
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::WindowResize, [gameEngine](Engine::EventData data) {
            SPDLOG_INFO("Window resized to {} x {}", data.window.width,
                        data.window.height);
            gameEngine->GetWindow().SetDimensions(data.window.width,
                                                  data.window.height);
        });
    /*
        gameEngine->GetEvents().RegisterCallback(
            Engine::EventType::KeyDown, [](Engine::EventData data) {
                SPDLOG_INFO("Key pressed: {} (keycode)", data.keyboard.keycode);
                SPDLOG_INFO("Key pressed: {} (scancode)", static_cast<unsigned
       int>(data.keyboard.scancode)); SPDLOG_INFO("Key pressed modifiers: {}",
       data.keyboard.modifiers);
            });
        gameEngine->GetEvents().RegisterCallback(
            Engine::EventType::KeyUp, [](Engine::EventData data) {
                SPDLOG_INFO("Key released: {} (keycode)",
       data.keyboard.keycode); SPDLOG_INFO("Key released: {} (scancode)",
       static_cast<unsigned int>(data.keyboard.scancode)); SPDLOG_INFO("Key
       released modifiers: {}", data.keyboard.modifiers);
            });
        */
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseButtonDown,
        [](Engine::EventData data) { SPDLOG_INFO("Mouse buttons pressed"); });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseWheel, [](Engine::EventData data) {
            SPDLOG_INFO("Mouse wheel scrolled: {:.0f}", data.mouse.y);
        });

    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseButtonUp,
        [](Engine::EventData data) { SPDLOG_INFO("Mouse buttons released."); });

    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::MouseMove, [](Engine::EventData data) {
            SPDLOG_INFO("Mouse moved to: ({:.0f}, {:.0f})", data.mouse.x,
                        data.mouse.y);
        });

    SPDLOG_INFO("Creating renderable objects.");
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

    SPDLOG_INFO("Adding renderable objects to render manager.");
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
                SPDLOG_INFO("Rectangle 1 selected");
            } else if (data.keyboard.keycode == SDLK_2) {
                s_currentRect = 2;
                SPDLOG_INFO("Rectangle 2 selected");
            } else if (data.keyboard.keycode == SDLK_3) {
                s_currentRect = 3;
                SPDLOG_INFO("Rectangle 3 selected");
            } else if (data.keyboard.keycode == SDLK_4) {
                s_currentRect = 4;
                SPDLOG_INFO("Rectangle 4 selected");
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
    SPDLOG_INFO("Object Rectangle controlled.");
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

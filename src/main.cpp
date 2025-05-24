#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <engine/engine.hpp>
#include <engine/logger.hpp>

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
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::KeyDown, [](Engine::EventData data) {
            SPDLOG_INFO("Key pressed: {} (keycode)", data.keyboard.keycode);
            SPDLOG_INFO("Key pressed: {} (scancode)",
                        static_cast<unsigned int>(data.keyboard.scancode));
            SPDLOG_INFO("Key pressed modifiers: {}", data.keyboard.modifiers);
        });
    gameEngine->GetEvents().RegisterCallback(
        Engine::EventType::KeyUp, [](Engine::EventData data) {
            SPDLOG_INFO("Key released: {} (keycode)", data.keyboard.keycode);
            SPDLOG_INFO("Key released: {} (scancode)",
                        static_cast<unsigned int>(data.keyboard.scancode));
            SPDLOG_INFO("Key released modifiers: {}", data.keyboard.modifiers);
        });
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

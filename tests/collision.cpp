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

    SPDLOG_INFO("Initializing game engine.");
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
    events.RegisterCallback(Engine::EventType::MouseMove,
                            [](Engine::EventData data) {
                                SPDLOG_INFO("Mouse moved to: ({:.0f}, {:.0f})",
                                            data.mouse.x, data.mouse.y);
                            });

    SPDLOG_INFO("Importing texture.");
    res.AddResource("tests/res/kenney-pixel-char1.png",
                    Engine::ResourceType::Texture);

    SPDLOG_INFO("Loading textures and creating renderable objects.");
    std::shared_ptr<Engine::Texture> tex1 =
        res.FindTexture("tests/res/kenney-pixel-char1.png");
    std::unique_ptr<Engine::Renderable> obj1 = std::make_unique<Engine::Sprite>(
        std::move(tex1), Engine::Vector2(100, 200));
    obj1->SetName("Character");
    std::unique_ptr<Engine::Renderable> obj2 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(10, 200, 60, 80),
                                                 Engine::Color::Green());
    obj2->SetName("Green Rectangle");
    std::unique_ptr<Engine::Renderable> obj3 =
        std::make_unique<Engine::RectangleShape>(Engine::Rect(900, 200, 60, 80),
                                                 Engine::Color::Red());
    obj3->SetName("Red Rectangle");

    SPDLOG_INFO("Adding renderable objects to render manager.");
    rdrMgr.AddRenderable(std::move(obj1), Engine::Layers::WORLD);
    rdrMgr.AddRenderable(std::move(obj2), Engine::Layers::ENTITIES);
    rdrMgr.AddRenderable(std::move(obj3), Engine::Layers::FOREGROUND);

    events.RegisterCallback(Engine::EventType::KeyDown,
                            [](Engine::EventData data) {
                                if (data.keyboard.keycode == SDLK_1) {
                                    s_currentObj = 1;
                                    SPDLOG_INFO("Character selected");
                                } else if (data.keyboard.keycode == SDLK_2) {
                                    s_currentObj = 2;
                                    SPDLOG_INFO("Green Rectangle selected");
                                } else if (data.keyboard.keycode == SDLK_3) {
                                    s_currentObj = 3;
                                    SPDLOG_INFO("Red Rectangle selected");
                                }
                            });

    auto getAABB = [](const Engine::Renderable *renderable,
                      const Engine::Vector2 &position) -> Engine::Rect {
        Engine::Vector2 scale = renderable->GetScale();
        Engine::Vector2 pivot = renderable->GetPivot();
        float scaledWidth = 0.0f;
        float scaledHeight = 0.0f;

        if (renderable->GetType() == Engine::RenderableType::Sprite) {
            const Engine::Sprite *sprite =
                static_cast<const Engine::Sprite *>(renderable);
            scaledWidth = sprite->GetSourceRect().w * scale.x;
            scaledHeight = sprite->GetSourceRect().h * scale.y;
        } else if (renderable->GetType() == Engine::RenderableType::Rectangle) {
            const Engine::RectangleShape *rectShape =
                static_cast<const Engine::RectangleShape *>(renderable);
            scaledWidth = rectShape->GetWidth() * scale.x;
            scaledHeight = rectShape->GetHeight() * scale.y;
        } else {
            // empty rect for other types for now
            return Engine::Rect(position.x, position.y, 0, 0);
        }

        float aabbX = position.x - (pivot.x * scaledWidth);
        float aabbY = position.y - (pivot.y * scaledHeight);

        return Engine::Rect(aabbX, aabbY, scaledWidth, scaledHeight);
    };

    auto checkCollision = [](const Engine::Rect &r1,
                             const Engine::Rect &r2) -> bool {
        const float epsilon = 1e-4f; // due to floating point errors
        return r1.x < r2.x + r2.w - epsilon && r1.x + r1.w > r2.x + epsilon &&
               r1.y < r2.y + r2.h - epsilon && r1.y + r1.h > r2.y + epsilon;
    };

    events.RegisterCallback(
        Engine::EventType::KeyDown,
        [&rdrMgr, getAABB, checkCollision](Engine::EventData data) {
            Engine::Renderable *obj = nullptr;
            Engine::Renderable *rect1 = rdrMgr.GetRenderable("Green Rectangle");
            Engine::Renderable *rect2 = rdrMgr.GetRenderable("Red Rectangle");
            Engine::Renderable *character = rdrMgr.GetRenderable("Character");
            switch (s_currentObj) {
            case 1:
                obj = character;
                break;
            case 2:
                obj = rect1;
                break;
            case 3:
                obj = rect2;
                break;
            }

            Engine::Vector2 move_delta = {0.0f, 0.0f};
            bool is_movement_key = false;

            if (data.keyboard.keycode == SDLK_RIGHT) {
                move_delta.x = 10.0f;
                is_movement_key = true;
            } else if (data.keyboard.keycode == SDLK_LEFT) {
                move_delta.x = -10.0f;
                is_movement_key = true;
            } else if (data.keyboard.keycode == SDLK_DOWN) {
                move_delta.y = 10.0f;
                is_movement_key = true;
            } else if (data.keyboard.keycode == SDLK_UP) {
                move_delta.y = -10.0f;
                is_movement_key = true;
            }

            if (is_movement_key) {
                Engine::Vector2 next_pos = obj->GetPosition() + move_delta;
                Engine::Rect obj_next_aabb = getAABB(obj, next_pos);
                // SPDLOG_TRACE("Moving object AABB (potential next pos): x={},
                // "
                //             "y={}, w={}, h={}",
                //             obj_next_aabb.x, obj_next_aabb.y,
                //             obj_next_aabb.w, obj_next_aabb.h);
                bool collision = false;
                if (obj != character && character) {
                    Engine::Rect char_current_aabb =
                        getAABB(character, character->GetPosition());
                    // SPDLOG_TRACE("Checking against Character AABB (current "
                    //             "pos): x={}, y={}, w={}, h={}",
                    //             char_current_aabb.x, char_current_aabb.y,
                    //             char_current_aabb.w, char_current_aabb.h);
                    if (checkCollision(obj_next_aabb, char_current_aabb)) {
                        collision = true;
                    }
                }
                if (!collision && obj != rect1 && rect1) {
                    Engine::Rect rect1_current_aabb =
                        getAABB(rect1, rect1->GetPosition());
                    // SPDLOG_TRACE("Checking against Green Rectangle AABB "
                    //             "(current pos): x={}, y={}, w={}, h={}",
                    //             rect1_current_aabb.x, rect1_current_aabb.y,
                    //             rect1_current_aabb.w, rect1_current_aabb.h);
                    if (checkCollision(obj_next_aabb, rect1_current_aabb)) {
                        collision = true;
                    }
                }
                if (!collision && obj != rect2 && rect2) {
                    Engine::Rect rect2_current_aabb =
                        getAABB(rect2, rect2->GetPosition());
                    // SPDLOG_TRACE("Checking against Red Rectangle AABB
                    // (current "
                    //             "pos): x={}, y={}, w={}, h={}",
                    //             rect2_current_aabb.x, rect2_current_aabb.y,
                    //             rect2_current_aabb.w, rect2_current_aabb.h);
                    if (checkCollision(obj_next_aabb, rect2_current_aabb)) {
                        collision = true;
                    }
                }
                if (!collision) {
                    obj->SetPosition(next_pos);
                } else {
                    SPDLOG_INFO("Collision prevented movement!");
                }
            }

            if (data.keyboard.keycode == SDLK_P) {
                obj->Scale(1.1f);
            } else if (data.keyboard.keycode == SDLK_Q) {
                obj->Scale(0.9f);
            } else if (data.keyboard.keycode == SDLK_SPACE) {
                int layer_id = s_currentObj * 100;
                if (rdrMgr.HasLayer(layer_id)) {
                    Engine::Layer &layer = rdrMgr.GetLayer(layer_id);
                    float opacity = layer.GetOpacity();
                    layer.SetOpacity(opacity + 0.1f);
                }
            } else if (data.keyboard.keycode == SDLK_LCTRL) {
                int layer_id = s_currentObj * 100;
                if (rdrMgr.HasLayer(layer_id)) {
                    Engine::Layer &layer = rdrMgr.GetLayer(layer_id);
                    float opacity = layer.GetOpacity();
                    layer.SetOpacity(opacity + 0.1f);
                }
            }
        });
    SPDLOG_INFO("Controls:\n1-3: Change selected object\n"
                "Arrow Keys: Move object\n"
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

cmake_minimum_required(VERSION 3.28...4.0)
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.10
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/include/vendored/SDL3
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/include/vendored/SDL3_ttf
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SDL3 SDL3_ttf)
FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/master.zip
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/include/vendored/imgui
  BINARY_DIR ${CMAKE_SOURCE_DIR}/_deps/imgui-build
)
add_library(imgui_sdl3
    STATIC
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui_sdl3 PUBLIC
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui
    ${CMAKE_SOURCE_DIR}/include/vendored/imgui/backends
)
target_link_libraries(imgui_sdl3 PRIVATE SDL3::SDL3)
set_target_properties(imgui_sdl3
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/_deps/imgui-build
)

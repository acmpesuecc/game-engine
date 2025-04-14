cmake_minimum_required(VERSION 3.16...4.0)
project(GameEngine
    VERSION 0.0.1
    DESCRIPTION "2D Game Engine built during AIEP 2025"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(GameEngine)
target_sources(GameEngine
    PRIVATE
    src/main.cpp
)
target_compile_features(GameEngine PUBLIC cxx_std_17)
target_compile_definitions(GameEngine PUBLIC SDL_MAIN_USE_CALLBACKS)
target_compile_options(GameEngine PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wno-unused-parameter>
)
target_link_libraries(GameEngine PRIVATE SDL3::SDL3)
set_target_properties(GameEngine
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

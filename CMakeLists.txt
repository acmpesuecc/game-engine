cmake_minimum_required(VERSION 3.16)
project(GameEngine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDL3_INCLUDE_DIR "C:/Users/vk360/Desktop/College/AIEP2025/SDL3-3.2.10/include")
set(SDL3_LIB_DIR "C:/Users/vk360/Desktop/College/AIEP2025/SDL3-3.2.10/lib/x64")
set(SDL3_DLL_PATH "C:/Users/vk360/Desktop/College/AIEP2025/SDL3-3.2.10/GameEngineClone/SDL3.dll")

add_executable(GameEngine)
target_sources(GameEngine PRIVATE "C:/Users/vk360/Desktop/College/AIEP2025/SDL3-3.2.10/GameEngineClone/src/main.cpp")

target_compile_features(GameEngine PUBLIC cxx_std_17)

target_compile_definitions(GameEngine PUBLIC SDL_MAIN_USE_CALLBACKS)

target_compile_options(GameEngine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wno-unused-parameter>
)

target_include_directories(GameEngine PRIVATE "${SDL3_INCLUDE_DIR}")
target_link_directories(GameEngine PRIVATE "${SDL3_LIB_DIR}")
target_link_libraries(GameEngine PRIVATE SDL3)

set_target_properties(GameEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


add_custom_command(TARGET GameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_DLL_PATH}"
    "$<TARGET_FILE_DIR:GameEngine>"
)

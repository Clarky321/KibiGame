cmake_minimum_required(VERSION 3.12)
project(KibiGame LANGUAGES C CXX)

# Общие настройки
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)  # Raylib требует C11

# Настройка raylib через FetchContent
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Подпроекты
add_subdirectory(KibiGameCore)
add_subdirectory(KibiGameEditor)

# Настройка стартового проекта для Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KibiGameEditor)
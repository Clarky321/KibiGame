cmake_minimum_required(VERSION 3.12)

project(KibiGameCore LANGUAGES CXX)

# Исходные файлы
set(ENGINE_HEADERS
    includes/KibiGameCore/test.h
)

set(ENGINE_SOURCES
    src/KibiGameCore/test.cpp
)

# Создание статической библиотеки
add_library(${PROJECT_NAME} STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# Настройка include-директорий
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
        $<INSTALL_INTERFACE:includes>
    PRIVATE
        src
)

# Линковка с raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Группировка файлов в IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} 
    FILES 
        ${ENGINE_HEADERS}
        ${ENGINE_SOURCES}
)
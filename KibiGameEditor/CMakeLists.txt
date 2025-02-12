cmake_minimum_required(VERSION 3.12)

project(KibiGameEditor LANGUAGES CXX)

# Исполняемый файл
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Линковка с ядром
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        KibiGameCore
        raylib  # Если редактор использует raylib напрямую
)

# Настройка выходной директории
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)
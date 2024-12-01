cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "FileExplorer")
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} "src/main.cpp")

# This project requires C++17 (std::filesystem)
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")

# Add GLFW (and OpenGL) to build
add_subdirectory("external/GLFW")
target_link_libraries(${PROJECT_NAME} glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# Add source files to build
set(IMGUI_PATH "external/imgui")
file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
    "${IMGUI_PATH}/imgui.cpp"
    "${IMGUI_PATH}/imgui_draw.cpp"
    "${IMGUI_PATH}/imgui_tables.cpp"
    "${IMGUI_PATH}/imgui_widgets.cpp"
    "${IMGUI_PATH}/imgui.h"
    "${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp"
    "${IMGUI_PATH}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_PATH}/backends/imgui_impl_opengl3.h"
    "${IMGUI_PATH}/backends/imgui_impl_glfw.h"
    "${IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp"
    "${IMGUI_PATH}/misc/cpp/imgui_stdlib.h"
)
target_sources(${PROJECT_NAME} PUBLIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC "${IMGUI_PATH}"
    PUBLIC "${IMGUI_PATH}"
    PUBLIC "external/GLFW/include/GLFW"
    PUBLIC "include/magic_enum"
)
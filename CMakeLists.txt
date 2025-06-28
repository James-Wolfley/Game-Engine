set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.15)
project(My2DEngine
    VERSION 0.1.0
    DESCRIPTION "A simple 2D ECS-based engine using GLFW, GLAD, and OpenGL"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW3 REQUIRED glfw3)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
include_directories(
    ${CMAKE_SOURCE_DIR}/include           # your public headers
    ${CMAKE_SOURCE_DIR}/third_party  # for third party librarys 
    ${GLFW3_INCLUDE_DIRS}                  # from pkg-config (e.g. /usr/include/GLFW)
    ${GLM_INCLUDE_DIRS} 
)

file(GLOB_RECURSE ENGINE_SOURCES
  ${CMAKE_SOURCE_DIR}/src/Engine/*.cpp
)
add_library(Engine STATIC
    ${ENGINE_SOURCES}
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)


add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c
)

target_link_libraries(Engine
    PUBLIC
        ${GLFW3_LIBRARIES}
        glad
        OpenGL::GL         )

file(GLOB_RECURSE GAME_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Game/main.cpp
)

add_executable(MyGame
    ${GAME_SOURCES}
)
target_link_libraries(MyGame
    PRIVATE
        Engine
        OpenGL::GL
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.15)
project(MyGame
    VERSION 0.1.0
    DESCRIPTION "A simple 2D ECS-based engine using GLFW, GLAD, and OpenGL"
    LANGUAGES C CXX
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_compile_definitions(_RELEASE _DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(_RELEASE)
else()
  add_compile_definitions(_DEBUG)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#require opengl
find_package(OpenGL REQUIRED)

# Add GLFW subdirectory (builds from source)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# Include directories
include_directories(
    include
    third_party/
)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES "src/Engine/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "src/Game/*.cpp")

# GLAD source (needs to be compiled)
set(GLAD_SOURCES third_party/glad/src/glad.c)

# Create executable
add_executable(${PROJECT_NAME} 
    ${ENGINE_SOURCES}
    ${GAME_SOURCES}
    ${GLAD_SOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()

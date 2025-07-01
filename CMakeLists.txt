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

include(ExternalProject)

ExternalProject_Add(glew_external
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/glew
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make -C <SOURCE_DIR> SYSTEM=linux-clang 
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1
)
# Create imported target for the built GLEW library
add_library(glew STATIC IMPORTED)
add_dependencies(glew glew_external)
set_target_properties(glew PROPERTIES
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/third_party/glew/lib/libGLEW.a
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/third_party/glew/include
  INTERFACE_COMPILE_DEFINITIONS GLEW_STATIC
)

# Add GLFW subdirectory (builds from source)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)


# Source files
file(GLOB_RECURSE ENGINE_SOURCES "src/Engine/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "src/Game/*.cpp")


# Create executable
add_executable(${PROJECT_NAME} 
    ${ENGINE_SOURCES}
    ${GAME_SOURCES}
    ${GLAD_SOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE 
   include
   third_party/
)
# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    glew
    ${CMAKE_DL_LIBS} 
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()

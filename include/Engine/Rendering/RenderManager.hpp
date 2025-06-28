#pragma once
#include "Engine/Core/Window.hpp"
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {

class RenderManager {
  public:
    RenderManager();
    ~RenderManager();

    void render(Engine::Window *window, float deltaTime);
    void renderUI(float deltaTime);

  private:
    Engine::Window *_window;
};

} // namespace Engine

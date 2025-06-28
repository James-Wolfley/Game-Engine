#include "Engine/Rendering/RenderManager.hpp"

namespace Engine {
RenderManager::RenderManager() {};
RenderManager::~RenderManager() {};

void RenderManager::render(Engine::Window *window, float deltaTime) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    _window = window;
    renderUI(deltaTime);
};
void RenderManager::renderUI(float deltaTime) {
    // printf("Rendering the UI\n");
};
} // namespace Engine

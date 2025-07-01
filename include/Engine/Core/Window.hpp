#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h> // include before any GL calls
#include <string>

namespace Engine {

class Window {
    static constexpr double TARGET_FPS = 240.0;
    static constexpr double TARGET_FRAME_DT = 1.0 / TARGET_FPS;

  public:
    Window(int width, int height, const std::string &title);
    ~Window();

    int getWindowHeight();
    int getWindowWidth();
    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void close();
    GLFWwindow *native() const {
        return window;
    }

  private:
    GLFWwindow *window = nullptr;
};
} // namespace Engine

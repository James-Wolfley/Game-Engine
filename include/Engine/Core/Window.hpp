#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h> // include before any GL calls
#include <string>

namespace Engine {

class Window {
public:
  Window(int width, int height, const std::string &title);
  ~Window();

  int getWindowHeight();
  int getWindowWidth();
  void pollEvents();
  void swapBuffers();
  bool shouldClose() const;
  void close();
  GLFWwindow *native() const { return window; }

private:
  GLFWwindow *window = nullptr;
};
} // namespace Engine

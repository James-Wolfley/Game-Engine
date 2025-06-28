#include "Engine/Core/Window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}
namespace Engine {

Window::Window(int width, int height, const std::string &title) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    // **Initialize GLAD here:**
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Set the viewport to the window size. We’ll also handle resizes below.
    // int w, h;
    // glfwGetFramebufferSize(window, &w, &h);
    // glViewport(0, 0, w, h);

    // If the window is resized, update the viewport automatically:
    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *window, int newW, int newH) {
                                       glViewport(0, 0, newW, newH);
                                   });

    glfwSwapInterval(0);
}

Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

int Window::getWindowHeight() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return height;
};

int Window::getWindowWidth() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return width;
};

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    if (window) {
        glfwSwapBuffers(window);
    }
}

bool Window::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void Window::close() {
    if (window) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

} // namespace Engine

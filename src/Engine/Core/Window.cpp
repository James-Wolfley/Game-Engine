#include "Engine/Core/Window.hpp"
#include "Engine/Core/ClockManager.hpp"
#include "Engine/Debug/DebugManager.hpp"
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

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    // If the window is resized, update the viewport automatically:
    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *window, int newW, int newH) { glViewport(0, 0, newW, newH); });

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
    double current_frame_delta = glfwGetTime() - CLOCK_MANAGER.RenderClock->getStartTime();
    double sleep_time = TARGET_FRAME_DT - current_frame_delta;
    if (sleep_time > 0.000) {                             // Only sleep if we have more than 1ms to
        CLOCK_MANAGER.precise_sleep(sleep_time * 1000.0); // Convert to milliseconds
    }

    current_frame_delta = glfwGetTime() - CLOCK_MANAGER.RenderClock->getStartTime();
    DEBUG_FPS(current_frame_delta);
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

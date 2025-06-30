#include "Engine/Core/Window.hpp"
#include "Engine/Debug/DebugManager.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <thread>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

constexpr double TARGET_FPS = 120.0;
constexpr double TARGET_FRAME_DT = 1.0 / TARGET_FPS;
constexpr double TICK_RATE = 20.0;
constexpr double TICK_DT = 1.0 / TICK_RATE;

int main() {
    Engine::Window window(800, 600, "Factory Game");

    double tick_lag = 0;
    double previous_frame_start = 0;
    auto &debug = Engine::Debug::DebugManager::getInstance();

    while (!window.shouldClose()) {
        double frame_start = glfwGetTime();
        double current_frame_delta = frame_start - previous_frame_start;
        previous_frame_start = frame_start;
        window.pollEvents();

        tick_lag += current_frame_delta;
        while (tick_lag >= TICK_DT) {
            tick_lag -= TICK_DT;
        }

        do {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(0.0000000001));
            // std::this_thread::yield();
            current_frame_delta = glfwGetTime() - frame_start;
        } while (current_frame_delta <= TARGET_FRAME_DT);
        DEBUG_FPS(current_frame_delta);
        std::cout << std::fixed << std::setprecision(10) << debug.getFPS()
                  << std::endl;
        window.swapBuffers();
    }
    return 0;
}

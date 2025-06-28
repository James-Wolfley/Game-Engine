#include "Engine/Core/Window.hpp"
#include "Engine/Debug/DebugManager.hpp"
#include "Engine/Rendering/RenderManager.hpp"
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <thread>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

constexpr double TARGET_FPS = 240.0;
constexpr double TARGET_FRAME_DT = 1.0 / TARGET_FPS;
constexpr double TICK_RATE = 20.0;
constexpr double TICK_DT = 1.0 / TICK_RATE;

int main() {
    Engine::Window window(800, 600, "Factory Game");
    Engine::RenderManager *render = new Engine::RenderManager();
    double last_time = glfwGetTime();
    double tick_lag = 0;
    double current_delta_time = 0;
    double frame_start = 0;
    double last_tick_time = 0;
    auto &debug = Engine::Debug::DebugManager::getInstance();
    while (!window.shouldClose()) {
        // timing
        frame_start = glfwGetTime();

        // Poll events than handle them.
        window.pollEvents();
        double current_time = glfwGetTime();
        current_delta_time = current_time - last_time;
        last_time = current_time;
        tick_lag += current_delta_time;
        // Do a fixed "Physics update"
        while (tick_lag >= TICK_DT) {
            tick_lag -= TICK_DT;
            double now = glfwGetTime();
            double tick_delta = now - last_tick_time;
            last_tick_time = now;
            DEBUG_UPS(tick_delta);
        }

        current_delta_time = glfwGetTime() - frame_start;
        // Render here
        render->render(&window, current_delta_time);

        // Do frame update here

        current_delta_time = glfwGetTime() - frame_start;
        // Sleep rest of frame time till its time to render a new frame.
        if (current_delta_time < TARGET_FRAME_DT) {
            double sleep_duration = TARGET_FRAME_DT - current_delta_time;
            std::this_thread::sleep_for(
                std::chrono::duration<double>(sleep_duration - 0.001));
        }
        do {
            current_delta_time = glfwGetTime() - frame_start;
        } while (current_delta_time <= TARGET_FRAME_DT);
        DEBUG_FPS(current_delta_time);
        // Swap the buffers to update the screen
        system("clear");
        std::cout << std::setprecision(2) << std::fixed << debug.getFPS()
                  << std::endl
                  << debug.getUPS() << std::endl;
        window.swapBuffers();
    }
    delete render;
    return 0;
}

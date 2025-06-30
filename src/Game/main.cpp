#include "Engine/Core/Window.hpp"
#include "Engine/Debug/DebugManager.hpp"
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
#ifdef _WIN32
struct TimerRAII {
    UINT period = 1;
    bool active = false;

    TimerRAII(UINT target_period = 1) {
        TIMECAPS tc;
        if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR) {
            period = max(target_period, tc.wPeriodMin);
            if (timeBeginPeriod(period) == TIMERR_NOERROR) {
                active = true;
                std::cout << "Timer resolution set to: " << period
                          << "ms (min: " << tc.wPeriodMin << "ms)" << std::endl;
            }
        }
    }

    ~TimerRAII() {
        if (active) {
            timeEndPeriod(period);
            std::cout << "Timer resolution restored" << std::endl;
        }
    }
};

#endif
void precise_sleep(double milliseconds) {
    if (milliseconds <= 0.0)
        return;

    auto target = std::chrono::high_resolution_clock::now() +
                  std::chrono::duration<double, std::milli>(milliseconds);

    if (milliseconds > 2) {
#ifdef _WIN32
        Sleep(static_cast<DWORD>(milliseconds - 1));
#else
        std::this_thread::sleep_for(
            std::chrono::duration<double, std::milli>(milliseconds - 1));
#endif
    }
    // Spin for final precision
    while (std::chrono::high_resolution_clock::now() < target) {
        std::this_thread::yield();
    }
}
} // namespace

constexpr double TARGET_FPS = 30.0;
constexpr double TARGET_FRAME_DT = 1.0 / TARGET_FPS;
constexpr double TICK_RATE = 20.0;
constexpr double TICK_DT = 1.0 / TICK_RATE;

int main() {
#ifdef _WIN32
    TimerRAII timer_guard(1);
#endif
    Engine::Window window(800, 600, "Factory Game");

    double tick_lag = 0;
    double previous_frame_start = 0;
    auto &debug = Engine::Debug::DebugManager::getInstance();

    while (!window.shouldClose()) {
        double frame_start = glfwGetTime();
        double current_frame_delta = frame_start - previous_frame_start;
        window.pollEvents();

        tick_lag += current_frame_delta;
        while (tick_lag >= TICK_DT) {
            tick_lag -= TICK_DT;
        }

        // Calculate remaining time to target frame duration
        current_frame_delta = glfwGetTime() - frame_start;
        double sleep_time = TARGET_FRAME_DT - current_frame_delta;
        if (sleep_time > 0.000) { // Only sleep if we have more than 1ms to
            precise_sleep(sleep_time * 1000.0); // Convert to milliseconds
        }

        current_frame_delta = glfwGetTime() - frame_start;
        DEBUG_FPS(current_frame_delta);
        std::cout << std::fixed << std::setprecision(10) << debug.getFPS()
                  << std::endl;
        window.swapBuffers();
        previous_frame_start = frame_start;
    }
    return 0;
}

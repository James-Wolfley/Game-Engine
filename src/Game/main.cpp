#include "Engine/Core/Window.hpp"
#include "Engine/Debug/DebugManager.hpp"
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <thread>
#include <windows.h>
#endif

namespace {
#ifdef _WIN32
struct TimerRAII {
    UINT period;
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

// Windows precise sleep using high resolution timer
void precise_sleep(double milliseconds) {
    if (milliseconds <= 0.0)
        return;

    auto start = std::chrono::high_resolution_clock::now();
    auto target =
        start + std::chrono::duration<double, std::milli>(milliseconds);

    // Sleep most of the time, spin for final precision
    double sleep_time = milliseconds - 0.5; // Leave 0.5ms for spin
    if (sleep_time > 0.0) {
        Sleep(static_cast<DWORD>(sleep_time));
    }

    // Spin for final precision
    while (std::chrono::high_resolution_clock::now() < target) {
        std::this_thread::yield();
    }
}
#else
// Dummy struct for non-Windows platforms
struct TimerRAII {
    TimerRAII(int = 1) {
        std::cout << "High resolution timer not available on this platform"
                  << std::endl;
    }
};

// Fallback sleep for non-Windows platforms
void precise_sleep(double milliseconds) {
    if (milliseconds <= 0.0)
        return;

    auto start = std::chrono::high_resolution_clock::now();
    auto target =
        start + std::chrono::duration<double, std::milli>(milliseconds);

    // Use standard sleep for most of the time
    double sleep_time =
        milliseconds - 1.0; // Leave 1ms for spin on other platforms
    if (sleep_time > 0.0) {
        std::this_thread::sleep_for(
            std::chrono::duration<double, std::milli>(sleep_time));
    }

    // Spin for final precision
    while (std::chrono::high_resolution_clock::now() < target) {
        std::this_thread::yield();
    }
}
#endif
} // namespace

constexpr double TARGET_FPS = 240.0;
constexpr double TARGET_FRAME_DT = 1.0 / TARGET_FPS;
constexpr double TICK_RATE = 20.0;
constexpr double TICK_DT = 1.0 / TICK_RATE;

int main() {
    Engine::Window window(800, 600, "Factory Game");
    TimerRAII timer_guard(1); 

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

        double before_sleep = glfwGetTime();
        // Calculate remaining time to target frame duration
        current_frame_delta = glfwGetTime() - frame_start;
        double sleep_time = TARGET_FRAME_DT - current_frame_delta;

        //if (sleep_time > 0.001) { // Only sleep if we have more than 1ms to wait
        //    precise_sleep(sleep_time * 1000.0); // Convert to milliseconds
        //}
        double real_sleep_time = before_sleep - glfwGetTime();

        do {
            current_frame_delta = glfwGetTime() - frame_start;
        } while (current_frame_delta <= TARGET_FRAME_DT);
        DEBUG_FPS(current_frame_delta);
        std::cout << std::fixed << std::setprecision(10) << debug.getFPS() << "  ||  " << real_sleep_time << std::endl;
        window.swapBuffers();
        previous_frame_start = frame_start;
    }
    return 0;
}

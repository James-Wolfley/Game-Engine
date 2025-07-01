#include "Engine/Core/ClockManager.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Debug/DebugManager.hpp"
#include <iomanip>
#include <iostream>
#include <stdlib.h>

static constexpr double TICK_RATE = 20.0;
static constexpr double TICK_DT = 1.0 / TICK_RATE;

int main() {
#ifdef _WIN32
    TimerRAII timer_guard(1);
#endif
#if defined _RELEASE && defined _DEBUG
    std::cout << "in release with debug info mode\n";
#elif _RELEASE
    std::cout << "in release mode\n";
#elif _DEBUG
    std::cout << "in debug mode\n";
#endif
    Engine::Core::ClockManager::GetInstance();
    Engine::Window window(800, 600, "Factory Game");

    double tick_lag;
    while (!window.shouldClose()) {
        CLOCK_MANAGER.UpdateClocks();
        window.pollEvents();

        tick_lag += CLOCK_MANAGER.GameClock->getStartTime() - CLOCK_MANAGER.GameClock->getLastTime();
        while (tick_lag >= TICK_DT) {
            tick_lag -= TICK_DT;
        }

        // Calculate remaining time to target frame duration
        window.swapBuffers();
        std::cout << std::fixed << std::setprecision(5) << DEBUG_GET_FPS << std::endl;
    }
    return 0;
}

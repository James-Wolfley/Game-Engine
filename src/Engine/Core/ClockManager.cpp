#include "Engine/Core/ClockManager.hpp"
#include <thread>

namespace Engine::Core {
ClockManager &ClockManager::GetInstance() {
    static ClockManager instance;
    return instance;
}
ClockManager::ClockManager() {
#ifdef _WIN32
    timer_guard = TimerRAII(1);
#endif
    GameClock = new EngineClock();
    RenderClock = new EngineClock();
};
ClockManager::~ClockManager() {
#ifdef _WIN32
    delete timer_guard;
#endif
    delete GameClock;
    delete RenderClock;
};

void ClockManager::precise_sleep(double milliseconds) {
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

void ClockManager::UpdateClocks() {
    GameClock->updateTime();
    RenderClock->updateTime();
};
} // namespace Engine::Core

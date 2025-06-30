// Engine/Core/ClockManager.cpp
#include "Engine/Core/ClockManager.hpp"
#include <thread>
#ifdef _WIN32
#include <iostream>
#endif
namespace Engine {
namespace Core {

ClockManager *ClockManager::instance = nullptr;

ClockManager::ClockManager() {
#ifdef _WIN32
    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR) {
        timer_period = std::max(1u, tc.wPeriodMin);
        if (timeBeginPeriod(timer_period) == TIMERR_NOERROR) {
            high_res_active = true;
            std::cout << "High resolution timing initialized: " << timer_period
                      << "ms resolution" << std::endl;
        }
    }
#endif
}

ClockManager::~ClockManager() {
#ifdef _WIN32
    if (high_res_active) {
        timeEndPeriod(timer_period);
        std::cout << "High resolution timing restored" << std::endl;
    }
#endif
}

ClockManager &ClockManager::getInstance() {
    if (!instance) {
        instance = new ClockManager();
    }
    return *instance;
}

void ClockManager::initialize() {
    getInstance(); // Ensures instance is created
}

void ClockManager::shutdown() {
    delete instance;
    instance = nullptr;
}

void ClockManager::preciseSleep(double milliseconds) {
    if (milliseconds <= 0.0)
        return;

    auto start = std::chrono::high_resolution_clock::now();
    auto target =
        start + std::chrono::duration<double, std::milli>(milliseconds);

#ifdef _WIN32
    double sleep_time = milliseconds - 0.5;
    if (sleep_time > 0.0) {
        Sleep(static_cast<DWORD>(sleep_time));
    }
#else
    double sleep_time = milliseconds - 1.0;
    if (sleep_time > 0.0) {
        std::this_thread::sleep_for(
            std::chrono::duration<double, std::milli>(sleep_time));
    }
#endif

    while (std::chrono::high_resolution_clock::now() < target) {
        std::this_thread::yield();
    }
}

double ClockManager::getCurrentTime() {
    static auto program_start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - program_start).count();
}

bool ClockManager::isHighResolutionAvailable() const {
#ifdef _WIN32
    return high_res_active;
#else
    return false;
#endif
}

} // namespace Core
} // namespace Engine

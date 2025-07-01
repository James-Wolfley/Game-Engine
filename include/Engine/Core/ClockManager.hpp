#pragma once
#include "Engine/Core/EngineClock.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Engine::Core {
class ClockManager {
  public:
#ifdef _WIN32
    struct TimerRAII {
        UINT period = 1;
        bool active = false;
        TimerRAII(UINT target_period = 1);
        ~TimerRAII();
    };
#endif
    static ClockManager &GetInstance();

    ClockManager(const ClockManager &) = delete;
    ClockManager &operator=(const ClockManager &) = delete;
    ClockManager(ClockManager &&) = delete;
    ClockManager &operator=(ClockManager &&) = delete;
    // constructors
    ~ClockManager();
    // member functions
    void precise_sleep(double milliseconds);
    void UpdateClocks();
    // member variables
    Engine::Core::EngineClock *GameClock;
    Engine::Core::EngineClock *RenderClock;

  private:
    ClockManager();
#ifdef _WIN32
    TimerRaII timer_guard;
#endif
};

#define CLOCK_MANAGER Engine::Core::ClockManager::GetInstance()
} // namespace Engine::Core

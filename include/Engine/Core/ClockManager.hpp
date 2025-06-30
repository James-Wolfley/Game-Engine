#pragma once

#ifdef _WIN32
#include <chrono>
#include <windows.h>
#endif

namespace Engine {
namespace Core {

class ClockManager {
  private:
#ifdef _WIN32
    UINT timer_period = 1;
    bool high_res_active = false;
#endif

    static ClockManager *instance;

  public:
    ClockManager();
    ~ClockManager();

    // Singleton access
    static ClockManager &getInstance();
    static void initialize();
    static void shutdown();

    // Disable copy/move
    ClockManager(const ClockManager &) = delete;
    ClockManager &operator=(const ClockManager &) = delete;

    // High precision sleep
    static void preciseSleep(double milliseconds);

    // Get current time in seconds
    static double getCurrentTime();

    // System info
    bool isHighResolutionAvailable() const;
};

} // namespace Core
} // namespace Engine

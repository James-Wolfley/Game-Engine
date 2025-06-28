// DebugManager.hpp
#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine::Debug {

class DebugManager {
public:
  // Singleton access
  static DebugManager &getInstance() {
    static DebugManager instance;
    return instance;
  }

  // Delete copy constructor and assignment operator
  DebugManager(const DebugManager &) = delete;
  DebugManager &operator=(const DebugManager &) = delete;

  // Performance metrics
  void updateUPS(float deltaTime);
  float getUPS() const { return currentUPS; }
  void updateFPS(float deltaTime);
  float getFPS() const { return currentFPS; }
  float getFrameTimeMs() const { return frameTimeMs; }
  float getAverageFrameTime() const { return averageFrameTime; }

  // Generic counters for various systems
  void incrementCounter(const std::string &name);
  void setCounter(const std::string &name, int value);
  int getCounter(const std::string &name) const;

  // Generic metrics (floats)
  void setMetric(const std::string &name, float value);
  float getMetric(const std::string &name) const;

  // String values for debug display
  void setDebugString(const std::string &name, const std::string &value);
  std::string getDebugString(const std::string &name) const;

  // Memory tracking
  void reportMemoryUsage(const std::string &category, size_t bytes);
  size_t getMemoryUsage(const std::string &category) const;

  // Timing utilities
  void startTimer(const std::string &name);
  void endTimer(const std::string &name);
  float getTimerMs(const std::string &name) const;

  // Configuration
  void setDebugVisible(bool visible) { debugVisible = visible; }
  bool isDebugVisible() const { return debugVisible; }

  // Reset all counters (useful for per-frame counters)
  void resetFrameCounters();

  // Get all debug info as formatted strings for display
  std::vector<std::string> getDebugLines() const;

private:
  DebugManager() = default;
  ~DebugManager() = default;

  // FPS tracking
  static constexpr int FPS_SAMPLE_COUNT = 60;
  float frameTimes[FPS_SAMPLE_COUNT] = {0.0f};
  int frameTimeIndex = 0;
  bool frameBufferFilled = false;
  float currentFPS = 0.0f;
  float frameTimeMs = 0.0f;
  float averageFrameTime = 0.0f;

  // ups metrics
  static constexpr int UPS_SAMPLE_COUNT = 60;
  float updateTimes[UPS_SAMPLE_COUNT] = {0.0f};
  int updateTimeIndex = 0;
  bool updateBuferFilled = false;
  float currentUPS = 0.0f;

  void updateUPSStats();

  // Generic storage
  std::unordered_map<std::string, int> counters;
  std::unordered_map<std::string, float> metrics;
  std::unordered_map<std::string, std::string> debugStrings;
  std::unordered_map<std::string, size_t> memoryUsage;

  // Timing
  std::unordered_map<std::string,
                     std::chrono::high_resolution_clock::time_point>
      activeTimers;
  std::unordered_map<std::string, float> timerResults; // in milliseconds

  bool debugVisible = true;

  void updateFrameTimeStats();
};

// Convenient macros for common operations
#define TOGGLE_DEBUG()                                                         \
  Engine::Debug::DebugManager::getInstance().setDebugVisible(                  \
      !Engine::Debug::DebugManager::getInstance().isDebugVisible())
#define DEBUG_FPS(deltaTime)                                                   \
  Engine::Debug::DebugManager::getInstance().updateFPS(deltaTime)
#define DEBUG_INCREMENT(name)                                                  \
  Engine::Debug::DebugManager::getInstance().incrementCounter(name)
#define DEBUG_SET_COUNTER(name, value)                                         \
  Engine::Debug::DebugManager::getInstance().setCounter(name, value)
#define DEBUG_SET_METRIC(name, value)                                          \
  Engine::Debug::DebugManager::getInstance().setMetric(name, value)
#define DEBUG_SET_STRING(name, value)                                          \
  Engine::Debug::DebugManager::getInstance().setDebugString(name, value)
#define DEBUG_START_TIMER(name)                                                \
  Engine::Debug::DebugManager::getInstance().startTimer(name)
#define DEBUG_END_TIMER(name)                                                  \
  Engine::Debug::DebugManager::getInstance().endTimer(name)
#define DEBUG_UPS(deltaTime)                                                   \
  Engine::Debug::DebugManager::getInstance().updateUPS(deltaTime)
} // namespace Engine::Debug

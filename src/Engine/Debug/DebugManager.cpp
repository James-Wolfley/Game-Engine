#include "Engine/Debug/DebugManager.hpp"
#include <iomanip>
#include <sstream>

namespace Engine::Debug {

void DebugManager::updateFPS(float deltaTime) {
  // Store frame time
  frameTimes[frameTimeIndex] = deltaTime;
  frameTimeIndex = (frameTimeIndex + 1) % FPS_SAMPLE_COUNT;

  if (frameTimeIndex == 0) {
    frameBufferFilled = true;
  }

  // Current frame time in ms
  frameTimeMs = deltaTime * 1000.0f;

  // Update statistics
  updateFrameTimeStats();
}

void DebugManager::updateFrameTimeStats() {
  int sampleCount = frameBufferFilled ? FPS_SAMPLE_COUNT : frameTimeIndex;
  if (sampleCount == 0)
    return;

  float totalTime = 0.0f;
  for (int i = 0; i < sampleCount; i++) {
    totalTime += frameTimes[i];
  }

  averageFrameTime = totalTime / sampleCount;
  currentFPS = 1.0f / averageFrameTime;
}

void DebugManager::updateUPS(float deltaTime) {
  updateTimes[updateTimeIndex] = deltaTime;
  updateTimeIndex = (updateTimeIndex + 1) % UPS_SAMPLE_COUNT;
  if (updateTimeIndex == 0) {
    updateBuferFilled = true;
  }

  updateUPSStats();
}

void DebugManager::updateUPSStats() {
  int sampleCount = updateBuferFilled ? UPS_SAMPLE_COUNT : updateTimeIndex;
  float totalTime = 0.0f;
  for (int i = 0; i < sampleCount; i++) {
    totalTime += updateTimes[i];
  }
  float averageUpdateTime = totalTime / sampleCount;
  currentUPS = 1.0f / averageUpdateTime;
}

void DebugManager::incrementCounter(const std::string &name) {
  counters[name]++;
}

void DebugManager::setCounter(const std::string &name, int value) {
  counters[name] = value;
}

int DebugManager::getCounter(const std::string &name) const {
  auto it = counters.find(name);
  return it != counters.end() ? it->second : 0;
}

void DebugManager::setMetric(const std::string &name, float value) {
  metrics[name] = value;
}

float DebugManager::getMetric(const std::string &name) const {
  auto it = metrics.find(name);
  return it != metrics.end() ? it->second : 0.0f;
}

void DebugManager::setDebugString(const std::string &name,
                                  const std::string &value) {
  debugStrings[name] = value;
}

std::string DebugManager::getDebugString(const std::string &name) const {
  auto it = debugStrings.find(name);
  return it != debugStrings.end() ? it->second : "";
}

void DebugManager::reportMemoryUsage(const std::string &category,
                                     size_t bytes) {
  memoryUsage[category] = bytes;
}

size_t DebugManager::getMemoryUsage(const std::string &category) const {
  auto it = memoryUsage.find(category);
  return it != memoryUsage.end() ? it->second : 0;
}

void DebugManager::startTimer(const std::string &name) {
  activeTimers[name] = std::chrono::high_resolution_clock::now();
}

void DebugManager::endTimer(const std::string &name) {
  auto it = activeTimers.find(name);
  if (it != activeTimers.end()) {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        endTime - it->second);
    timerResults[name] = duration.count() / 1000.0f; // Convert to milliseconds
    activeTimers.erase(it);
  }
}

float DebugManager::getTimerMs(const std::string &name) const {
  auto it = timerResults.find(name);
  return it != timerResults.end() ? it->second : 0.0f;
}

void DebugManager::resetFrameCounters() {
  // Reset counters that should be cleared each frame
  // You can add specific counter names here that should reset
  for (auto &pair : counters) {
    if (pair.first.find("frame_") ==
        0) { // Reset counters starting with "frame_"
      pair.second = 0;
    }
  }
}

std::vector<std::string> DebugManager::getDebugLines() const {
  std::vector<std::string> lines;

  // Performance metrics
  {
    std::ostringstream oss;
    oss << "FPS: " << std::fixed << std::setprecision(1) << currentFPS;
    lines.push_back(oss.str());
  }

  {
    std::ostringstream oss;
    oss << "Frame Time: " << std::fixed << std::setprecision(2) << frameTimeMs
        << "ms";
    lines.push_back(oss.str());
  }

  {
    std::ostringstream oss;
    oss << "UPS: " << std::fixed << std::setprecision(1) << currentUPS;
    lines.push_back(oss.str());
  }

  // Custom counters
  for (const auto &pair : counters) {
    std::ostringstream oss;
    oss << pair.first << ": " << pair.second;
    lines.push_back(oss.str());
  }

  // Custom metrics
  for (const auto &pair : metrics) {
    std::ostringstream oss;
    oss << pair.first << ": " << std::fixed << std::setprecision(2)
        << pair.second;
    lines.push_back(oss.str());
  }

  // Debug strings
  for (const auto &pair : debugStrings) {
    lines.push_back(pair.first + ": " + pair.second);
  }

  // Memory usage
  for (const auto &pair : memoryUsage) {
    std::ostringstream oss;
    float mb = pair.second / (1024.0f * 1024.0f);
    oss << pair.first << " Memory: " << std::fixed << std::setprecision(1) << mb
        << "MB";
    lines.push_back(oss.str());
  }

  // Timer results
  for (const auto &pair : timerResults) {
    std::ostringstream oss;
    oss << pair.first << " Time: " << std::fixed << std::setprecision(2)
        << pair.second << "ms";
    lines.push_back(oss.str());
  }

  return lines;
}

} // namespace Engine::Debug

#include "Engine/Core/EngineClock.hpp"
#include "GLFW/glfw3.h"

namespace Engine::Core {

EngineClock::EngineClock() {
    currentTime = glfwGetTime();
};

void EngineClock::updateTime() {
    if (!isPaused) {
        lastTime = currentTime;
        startTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        if (startTime - lastTime > maxDelta) {
            lastTime = startTime - maxDelta;
        }
    }
};
void EngineClock::stepForward() {
    startTime += timeStep;
    lastTime += timeStep;
};
bool EngineClock::IsPaused() const {
    return isPaused;
};
void EngineClock::PauseClock() {
    isPaused = true;
};
void EngineClock::UnPauseClock() {
    isPaused = false;
};
double EngineClock::getMaxDelta() const {
    return maxDelta;
};
void EngineClock::setMaxDelta(double value) {
    maxDelta = value;
};
double EngineClock::getLastTime() const {
    return lastTime;
};
double EngineClock::getStartTime() const {
    return startTime;
};
double EngineClock::getDeltaTime() const {
    return deltaTime;
};
double EngineClock::getTimeStep() const {
    return timeStep;
};
void EngineClock::setTimeStep(double value) {
    timeStep = value;
};
} // namespace Engine::Core

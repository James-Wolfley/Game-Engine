#pragma once
namespace Engine::Core {
class EngineClock {
  public:
    EngineClock();
    ~EngineClock() = default;

    void updateTime();
    void stepForward();
    bool IsPaused() const;
    void PauseClock();
    void UnPauseClock();
    double getMaxDelta() const;
    void setMaxDelta(double value);
    double getLastTime() const;
    double getStartTime() const;
    double getDeltaTime() const;
    double getTimeStep() const;
    void setTimeStep(double value);

  private:
    bool isPaused = false;
    double currentTime = 0;
    double maxDelta = 1.0;
    double lastTime = 0;
    double startTime = 0;
    double deltaTime = 0;
    double timeStep = 0.050; // time step for single steping the clock
};
} // namespace Engine::Core

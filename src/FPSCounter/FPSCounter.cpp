#include "FPSCounter.hpp"

FPSCounter::FPSCounter() : fps(0.0), frameTime(0.0) {}

FPSCounter::~FPSCounter() {}

void FPSCounter::startFrame() {
    startTime = std::chrono::steady_clock::now();
}

void FPSCounter::endFrame() {
    endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    frameTime = diff.count();
    fps = 1.0 / frameTime;
}

double FPSCounter::getFPS() const {
    return fps;
}
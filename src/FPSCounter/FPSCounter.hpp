#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <opencv2/opencv.hpp>
#include <chrono>

class FPSCounter
{
public:
    FPSCounter();
    ~FPSCounter();

    void startFrame();            // Call at the start of each frame
    void endFrame();              // Call at the end of each frame
    double getFPS() const;        // Get the current FPS
    void drawFPS(cv::Mat &frame); // Draw the FPS on the given frame

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> endTime;
    double fps;
    double frameTime;
};

#endif // FPSCOUNTER_HPP
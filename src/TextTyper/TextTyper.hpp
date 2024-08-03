#ifndef TEXT_TYPER_HPP
#define TEXT_TYPER_HPP

#include <opencv2/opencv.hpp>
#include <string>

class TextTyper
{
public:
    static void typeTextSlowly(const std::string &text, int delayMs);
    static void typeTextSlowlyOnScreen(cv::Mat &frame, const std::string &text, int delayMs, int &currentIndex, std::chrono::steady_clock::time_point &lastUpdateTime);
};

#endif // TEXT_TYPER_HPP
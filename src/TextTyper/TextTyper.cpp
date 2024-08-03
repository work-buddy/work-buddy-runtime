#include "TextTyper.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

void TextTyper::typeTextSlowly(const std::string &text, int delayMs)
{
    for (char c : text)
    {
        cout << c << flush;
        std::this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}

void TextTyper::typeTextSlowlyOnScreen(cv::Mat &frame, const std::string &text, int delayMs, int &currentIndex, std::chrono::steady_clock::time_point &lastUpdateTime)
{
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    int thickness = 2;
    int baseline = 0;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

    if (elapsed >= delayMs && currentIndex < text.size())
    {
        lastUpdateTime = now;
        currentIndex++;
    }

    cv::Mat buffer = frame.clone();

    cv::Size textSize = cv::getTextSize(text.substr(0, currentIndex), fontFace, fontScale, thickness, &baseline);
    cv::Point textOrg((buffer.cols - textSize.width) / 2, (buffer.rows + textSize.height) / 2);

    cv::putText(buffer, text.substr(0, currentIndex), textOrg, fontFace, fontScale, cv::Scalar(0, 255, 0), thickness);

    buffer.copyTo(frame);
}
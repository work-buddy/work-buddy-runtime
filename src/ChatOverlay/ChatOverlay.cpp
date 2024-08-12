#include "ChatOverlay.hpp"

ChatOverlay::ChatOverlay(int width, int height) : width(width), height(height), maxMessages(5) {}

void ChatOverlay::addMessage(const std::string &sender, const std::string &message)
{
    messages.push_back({sender, message});
    if (messages.size() > maxMessages)
    {
        messages.erase(messages.begin());
    }
}

void ChatOverlay::render(cv::Mat &frame)
{
    int yOffset = frame.rows - 20; // Start from the bottom of the frame
    int padding = 5; // Padding around the text

    for (auto it = messages.rbegin(); it != messages.rend(); ++it)
    {
        std::string text = it->first + ": " + it->second;
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.7, 2, &baseline);

        // Draw background rectangle
        cv::rectangle(frame, cv::Point(5, yOffset - textSize.height - padding),
                      cv::Point(5 + textSize.width + padding * 2, yOffset + baseline + padding),
                      cv::Scalar(0, 0, 0), cv::FILLED);

        // Draw text
        cv::putText(frame, text, cv::Point(10, yOffset), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
        yOffset -= (textSize.height + padding * 2 + 10); // Adjust yOffset for the next message
    }
}
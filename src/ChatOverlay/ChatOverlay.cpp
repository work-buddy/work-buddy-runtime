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

std::vector<std::string> ChatOverlay::wrapText(const std::string &text, int maxWidth, double fontScale, int thickness)
{
    std::vector<std::string> wrappedLines;
    std::string currentLine;
    std::istringstream stream(text);
    std::string word;

    while (stream >> word)
    {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(testLine, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);

        if (textSize.width > maxWidth)
        {
            if (!currentLine.empty())
            {
                wrappedLines.push_back(currentLine);
                currentLine = word;
            }
            else
            {
                wrappedLines.push_back(testLine);
                currentLine.clear();
            }
        }
        else
        {
            currentLine = testLine;
        }
    }

    if (!currentLine.empty())
    {
        wrappedLines.push_back(currentLine);
    }

    return wrappedLines;
}

void ChatOverlay::render(cv::Mat &frame)
{
    int yOffset = frame.rows - 20; // Start from the bottom of the frame
    int padding = 5;               // Padding around the text
    double fontScale = 0.7;
    int thickness = 2;

    for (auto it = messages.rbegin(); it != messages.rend(); ++it)
    {
        std::string fullText = it->first + ": " + it->second;
        std::vector<std::string> wrappedLines = wrapText(fullText, frame.cols - 20, fontScale, thickness);

        for (auto wrappedIt = wrappedLines.rbegin(); wrappedIt != wrappedLines.rend(); ++wrappedIt)
        {
            int baseline = 0;
            cv::Size textSize = cv::getTextSize(*wrappedIt, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);

            // Draw background rectangle
            cv::rectangle(frame, cv::Point(5, yOffset - textSize.height - padding),
                          cv::Point(5 + textSize.width + padding * 2, yOffset + baseline + padding),
                          cv::Scalar(0, 0, 0), cv::FILLED);

            // Draw text
            cv::putText(frame, *wrappedIt, cv::Point(10, yOffset), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(255, 255, 255), thickness);
            yOffset -= (textSize.height + padding * 2 + 10); // Adjust yOffset for the next line
        }
    }
}

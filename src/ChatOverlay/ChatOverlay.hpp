#ifndef CHATOVERLAY_HPP
#define CHATOVERLAY_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class ChatOverlay
{
public:
    ChatOverlay(int width, int height);

    void addMessage(const std::string &sender, const std::string &message);
    void render(cv::Mat &frame);

private:
    int width;
    int height;
    int maxMessages;
    std::vector<std::pair<std::string, std::string>> messages;
    std::vector<std::string> wrapText(const std::string &text, int maxWidth, double fontScale, int thickness);
};

#endif // CHATOVERLAY_HPP

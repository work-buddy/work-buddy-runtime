#ifndef CHAT_OVERLAY_HPP
#define CHAT_OVERLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class ChatOverlay
{
public:
    ChatOverlay(int width, int height);
    void addMessage(const std::string &sender, const std::string &message);
    void render(cv::Mat &frame);

private:
    int width;
    int height;
    std::vector<std::pair<std::string, std::string>> messages;
    int maxMessages;
};

#endif // CHAT_OVERLAY_HPP
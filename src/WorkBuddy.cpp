#include "FPSCounter/FPSCounter.hpp"
// #include "YoloDetector/YoloDetector.hpp"
#include "ORBTracker/ORBTracker.hpp"
#include "TextTyper/TextTyper.hpp"
#include "OpenAI/OpenAI.hpp"
#include "ChatOverlay.hpp"
#include <./nlohmann/json.hpp>

#include <opencv2/opencv.hpp>
#include <thread>

using json = nlohmann::json;
using namespace std;

int main()
{
    cv::VideoCapture videoCapture(0);

    if (!videoCapture.isOpened())
    {
        std::cerr << "Error opening video capture" << std::endl;
        return -1;
    }

    const char *openAIKey = std::getenv("OPENAI_KEY");
    if (openAIKey == nullptr)
    {
        std::cerr << "Error: OPENAI_KEY environment variable is not set." << std::endl;
        return 1;
    }

    FPSCounter fpsCounter;

    OpenAI openAI(openAIKey);

    ChatOverlay chatOverlay(640, 480);
    chatOverlay.addMessage("System", "Work Buddy HUD Online");

    std::string model = "gpt-4o-mini";
    std::vector<std::pair<std::string, std::string>> messages = {
        {"user", "You are Buddy, and XR AI assistant running in the user's glasses. You can help the user with various tasks like object detection, tracking, and chat. Greet the user in a short sentence to start."},
    };
    double temperature = 0.7;

    std::string openAIResponse = openAI.chatCompletion(model, messages, temperature);

    json openAIJSON = json::parse(openAIResponse);
    std::string greeting = openAIJSON["choices"][0]["message"]["content"];

    chatOverlay.addMessage("Buddy AI", greeting);

    std::string modelPath = "models/onnx/yolov8x.onnx";
    std::string classesFile = "datasets/Coco.names";

    // YoloDetector detector(modelPath, 0.5, 0.5);
    // detector.loadClasses(classesFile);

    ORBTracker tracker;

    cv::Mat frame;
    int frameDelay = 60;

    while (true)
    {
        fpsCounter.startFrame();

        videoCapture >> frame;
        if (frame.empty())
            break;

        try
        {
            tracker.processFrame(frame);
            tracker.drawKeypoints(frame, tracker.getKeypoints());
        }
        catch (const cv::Exception &e)
        {
            std::cerr << "Detector / Tracker error: " << e.what() << std::endl;
            break;
        }

        cv::Mat frameWithKeypoints = frame.clone();

        chatOverlay.render(frame);

        fpsCounter.endFrame();
        fpsCounter.drawFPS(frame);

        cv::imshow("Work Buddy", frame);

        if (cv::waitKey(1) >= 0)
        {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay));
    }

    return 0;
}
#include "FPSCounter/FPSCounter.hpp"
// #include "YoloDetector/YoloDetector.hpp"
#include "ORBTracker/ORBTracker.hpp"
#include "TextTyper/TextTyper.hpp"
#include "OpenAI/OpenAI.hpp"
#include "ChatOverlay/ChatOverlay.hpp"
#include "PythonSocket/PythonSocket.hpp"

#include <./nlohmann/json.hpp>
#include "utils/Base64/Base64.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include <opencv2/opencv.hpp>
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

    try
    {
        PythonSocket pythonSocket("127.0.0.1", 5000);
        pythonSocket.connect();

        std::string socketRequest = "GET /pyannote pyannote/harry-potter.wav";
        pythonSocket.send(socketRequest);

        std::string response = pythonSocket.receive();
        std::cout << "PythonSocket Response: " << response << std::endl;

        pythonSocket.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "PythonSocket error: " << e.what() << std::endl;
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

    std::string modelPath = "models/onnx/yolov8x.onnx";
    std::string classesFile = "datasets/Coco.names";

    // YoloDetector detector(modelPath, 0.5, 0.5);
    // detector.loadClasses(classesFile);

    ORBTracker tracker;

    std::string model = "gpt-4o-mini";
    std::vector<std::pair<std::string, std::string>> messages = {
        {"user", "You are Buddy, an XR AI assistant running in the user's glasses. You can help the user with various tasks like object detection, tracking, and chat. Greet the user."},
    };
    double temperature = 0.7;

    std::string openAIResponse = openAI.chatCompletion(model, messages, temperature);

    json openAIJSON = json::parse(openAIResponse);
    std::string greeting = openAIJSON["choices"][0]["message"]["content"];

    chatOverlay.addMessage("Buddy AI", greeting);

    cv::Mat frame;
    int frameDelay = 60;

    auto lastSentTime = std::chrono::steady_clock::now();

    while (true)
    {
        fpsCounter.startFrame();

        videoCapture >> frame;
        if (frame.empty())
            break;

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastSentTime).count();

        if (elapsedTime >= 15)
        {
            std::vector<std::pair<std::string, std::string>> messages = {
                {"user", "Attached is an image from the user's glasses. Can you describe what you see?"},
            };
            std::string base64Image = base64Encode(frame);

            openAI.chatCompletionAsync(model, messages, temperature, base64Image,
                                       [&](const std::string &response)
                                       {
                                           json openAIJSON = json::parse(response);
                                           std::string aiMessage = openAIJSON["choices"][0]["message"]["content"];
                                           chatOverlay.addMessage("Buddy AI", aiMessage);
                                       });
            lastSentTime = currentTime;
        }

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

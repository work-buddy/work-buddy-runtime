#include "FPSCounter/FPSCounter.hpp"
#include "YoloDetector/YoloDetector.hpp"
#include "ORBTracker/ORBTracker.hpp"
#include "TextTyper/TextTyper.hpp"

#include <opencv2/opencv.hpp>
#include <thread>

using namespace std;

int main()
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        return -1;
    }

    FPSCounter fpsCounter;

    std::string modelPath = "models/onnx/yolov8x.onnx";
    std::string classesFile = "datasets/Coco.names";

    YoloDetector detector(modelPath, 0.5, 0.5);

    detector.loadClasses(classesFile);

    ORBTracker tracker;

    int currentIndex = 0;
    std::string introText = "Work Buddy HUD Online";

    cv::Mat frame;
    const int frameDelayMs = 33; // Approx. 30 FPS
    auto lastUpdateTime = std::chrono::steady_clock::now();

    while (true)
    {
        fpsCounter.startFrame();

        cap >> frame;
        if (frame.empty())
            break;

        if (currentIndex < introText.size())
        {
            TextTyper::typeTextSlowlyOnScreen(frame, introText, 100, currentIndex, lastUpdateTime);
        }

        cv::Mat frameWithKeypoints = frame.clone();

        tracker.processFrame(frame);
        // detector.detectAndDraw(frame);
        tracker.drawKeypoints(frame, tracker.getKeypoints());

        fpsCounter.endFrame();
        fpsCounter.drawFPS(frame);

        cv::imshow("Work Buddy", frame);

        if (cv::waitKey(1) >= 0)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMs));
    }

    return 0;
}
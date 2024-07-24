#include <opencv2/opencv.hpp>
#include "FPSCounter/FPSCounter.hpp"

using namespace std;
int main()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        return -1;
    }

    FPSCounter fpsCounter;

    cv::Mat frame;
    while (true)
    {
        fpsCounter.startFrame();

        cap >> frame;
        if (frame.empty())
            break;

        fpsCounter.endFrame();
        double fps = fpsCounter.getFPS();
        std::string fpsText = "FPS: " + std::to_string(fps);

        cv::putText(frame, fpsText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 255, 0), 2);

        cv::imshow("Frame", frame);

        std::cout << "FPS: " << fps << std::endl;

        if (cv::waitKey(30) >= 0)
            break;
    }

    return 0;
}
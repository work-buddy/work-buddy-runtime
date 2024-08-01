#include "FPSCounter/FPSCounter.hpp"
#include "YoloDetector/YoloDetector.hpp"
#include <opencv2/opencv.hpp>

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

    YoloDetector detector(modelPath, classesFile);

    cv::Mat frame;

    while (true)
    {
        fpsCounter.startFrame();

        cap >> frame;
        if (frame.empty())
            break;

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        detector.detect(frame, classIds, confidences, boxes);

        for (size_t i = 0; i < boxes.size(); ++i)
        {
            detector.drawPrediction(classIds[i], confidences[i], boxes[i].x, boxes[i].y, boxes[i].x + boxes[i].width, boxes[i].y + boxes[i].height, frame);
        }

        fpsCounter.endFrame();
        fpsCounter.drawFPS(frame);

        cv::imshow("Work Buddy", frame);

        if (cv::waitKey(30) >= 0)
            break;
    }

    return 0;
}
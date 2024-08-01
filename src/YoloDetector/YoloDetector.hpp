#ifndef YOLO_DETECTOR_HPP
#define YOLO_DETECTOR_HPP

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

class YoloDetector
{
public:
    YoloDetector(const std::string &modelPath, const std::string &classesFilePath, float confThreshold = 0.5f, float nmsThreshold = 0.4f);
    void detect(const cv::Mat &frame, std::vector<int> &classIds, std::vector<float> &confidences, std::vector<cv::Rect> &boxes);
    void drawPrediction(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame);

private:
    void getClasses(const std::string &classesFilePath);
    void postProcess(const cv::Mat &frame, const std::vector<cv::Mat> &outs, std::vector<int> &classIds, std::vector<float> &confidences, std::vector<cv::Rect> &boxes);

    cv::dnn::Net net_;
    std::vector<std::string> classes_;
    float confThreshold_;
    float nmsThreshold_;
    int inputWidth_;
    int inputHeight_;
    float scale_;
    cv::Scalar mean_;
    bool swapRB_;
};

#endif // YOLO_DETECTOR_HPP

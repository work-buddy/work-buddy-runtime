#ifndef YOLO_DETECTOR_HPP
#define YOLO_DETECTOR_HPP

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <string>
#include <vector>

class YoloDetector {
public:
    YoloDetector(const std::string& modelPath, float confThreshold, float nmsThreshold);
    void loadClasses(const std::string& classesFile);
    void detectAndDraw(cv::Mat& frame);
    
private:
    void drawPrediction(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);
    void yoloPostProcessing(const std::vector<cv::Mat>& outs, std::vector<int>& keep_classIds, std::vector<float>& keep_confidences, std::vector<cv::Rect2d>& keep_boxes);

    cv::dnn::Net net;
    std::vector<std::string> classes;
    float confThreshold;
    float nmsThreshold;
};

#endif // YOLO_DETECTOR_HPP

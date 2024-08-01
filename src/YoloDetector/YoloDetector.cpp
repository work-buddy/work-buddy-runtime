#include "YoloDetector.hpp"
#include <fstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>

YoloDetector::YoloDetector(const std::string &modelPath, const std::string &classesFilePath, float confThreshold, float nmsThreshold)
    : confThreshold_(confThreshold), nmsThreshold_(nmsThreshold), inputWidth_(640), inputHeight_(640), scale_(1 / 255.0), mean_(0, 0, 0), swapRB_(true)
{
    net_ = cv::dnn::readNet(modelPath);

    net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);

    getClasses(classesFilePath);
}

void YoloDetector::getClasses(const std::string &classesFilePath)
{
    std::ifstream ifs(classesFilePath.c_str());
    if (!ifs.is_open())
    {
        throw std::runtime_error("File " + classesFilePath + " not found");
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        classes_.push_back(line);
    }
}

void YoloDetector::detect(const cv::Mat &frame, std::vector<int> &classIds, std::vector<float> &confidences, std::vector<cv::Rect> &boxes)
{
    cv::Mat blob;

    // Debugging: Print input frame size and type
    std::cout << "Input frame size: " << frame.size() << ", type: " << frame.type() << std::endl;

    // Create a 4D blob from the image
    cv::dnn::blobFromImage(frame, blob, scale_, cv::Size(inputWidth_, inputHeight_), mean_, swapRB_, false);

    // Debugging: Print blob size
    std::cout << "Blob size: " << blob.size << std::endl;

    net_.setInput(blob);

    // Forward pass
    std::vector<cv::Mat> outs;
    net_.forward(outs, net_.getUnconnectedOutLayersNames());

    // Debugging: Print the number of outputs and their shapes
    std::cout << "Number of outputs: " << outs.size() << std::endl;
    for (size_t i = 0; i < outs.size(); ++i)
    {
        std::cout << "Output " << i << " shape: " << outs[i].size << std::endl;
    }

    postProcess(frame, outs, classIds, confidences, boxes);
}

void YoloDetector::postProcess(const cv::Mat &frame, const std::vector<cv::Mat> &outs, std::vector<int> &classIds, std::vector<float> &confidences, std::vector<cv::Rect> &boxes)
{
    std::vector<int> indices;
    for (size_t i = 0; i < outs.size(); ++i)
    {
        float *data = (float *)outs[i].data;
        for (int j = 0; j < outs[i].size[2]; ++j, data += outs[i].size[1])
        {
            float confidence = data[4]; // Objectness score
            if (confidence >= confThreshold_)
            {
                cv::Mat scores(1, outs[i].size[1] - 5, CV_32F, data + 5);
                cv::Point classIdPoint;
                double maxClassScore;
                cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);

                if (maxClassScore > confThreshold_)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)maxClassScore);
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }
    }

    // Apply Non-Maximum Suppression to eliminate redundant overlapping boxes with lower confidences
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold_, nmsThreshold_, indices);

    std::vector<cv::Rect> nmsBoxes;
    std::vector<int> nmsClassIds;
    std::vector<float> nmsConfidences;
    for (auto idx : indices)
    {
        nmsBoxes.push_back(boxes[idx]);
        nmsClassIds.push_back(classIds[idx]);
        nmsConfidences.push_back(confidences[idx]);
    }
    boxes = nmsBoxes;
    classIds = nmsClassIds;
    confidences = nmsConfidences;
}

void YoloDetector::drawPrediction(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame)
{
    rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 255, 0));

    std::string label = cv::format("%.2f", conf);
    if (!classes_.empty() && classId < (int)classes_.size())
    {
        label = classes_[classId] + ": " + label;
    }

    int baseLine;
    cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = cv::max(top, labelSize.height);
    rectangle(frame, cv::Point(left, top - labelSize.height), cv::Point(left + labelSize.width, top + baseLine), cv::Scalar::all(255), cv::FILLED);
    putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar());
}

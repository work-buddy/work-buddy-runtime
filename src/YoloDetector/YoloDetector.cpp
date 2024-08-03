#include "YoloDetector.hpp"

YoloDetector::YoloDetector(const std::string &modelPath, float confThreshold, float nmsThreshold)
    : confThreshold(confThreshold), nmsThreshold(nmsThreshold)
{
    net = cv::dnn::readNet(modelPath);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
}

void YoloDetector::loadClasses(const std::string &classesFile)
{
    std::ifstream ifs(classesFile.c_str());
    if (!ifs.is_open())
        CV_Error(cv::Error::StsError, "File " + classesFile + " not found");
    std::string line;
    while (std::getline(ifs, line))
        classes.push_back(line);
}

void YoloDetector::drawPrediction(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame)
{
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 255, 0));
    std::string label = cv::format("%.2f", conf);
    if (!classes.empty() && classId < classes.size())
    {
        label = classes[classId] + ": " + label;
    }
    int baseLine;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    cv::rectangle(frame, cv::Point(left, top - labelSize.height),
                  cv::Point(left + labelSize.width, top + baseLine), cv::Scalar::all(255), cv::FILLED);
    cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar());
}

void YoloDetector::yoloPostProcessing(const std::vector<cv::Mat> &outs, std::vector<int> &keep_classIds, std::vector<float> &keep_confidences, std::vector<cv::Rect2d> &keep_boxes)
{
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect2d> boxes;

    for (const auto &preds : outs)
    {
        for (int i = 0; i < preds.rows; ++i)
        {
            float obj_conf = preds.at<float>(i, 4);
            if (obj_conf < confThreshold)
                continue;

            cv::Mat scores = preds.row(i).colRange(5, preds.cols);
            double conf;
            cv::Point maxLoc;
            cv::minMaxLoc(scores, 0, &conf, 0, &maxLoc);

            conf *= obj_conf;
            if (conf < confThreshold)
                continue;

            const float *det = preds.ptr<float>(i);
            double cx = det[0];
            double cy = det[1];
            double w = det[2];
            double h = det[3];

            boxes.push_back(cv::Rect2d(cx - 0.5 * w, cy - 0.5 * h, cx + 0.5 * w, cy + 0.5 * h));
            classIds.push_back(maxLoc.x);
            confidences.push_back(static_cast<float>(conf));
        }
    }

    std::vector<int> keep_idx;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, keep_idx);

    for (auto i : keep_idx)
    {
        keep_classIds.push_back(classIds[i]);
        keep_confidences.push_back(confidences[i]);
        keep_boxes.push_back(boxes[i]);
    }
}

void YoloDetector::detectAndDraw(cv::Mat &frame)
{
    cv::Size size(640, 640);        // Adjust size as per the model's requirement
    cv::Scalar mean(0.0, 0.0, 0.0); // Adjust mean if necessary
    float scale = 1.0 / 255.0;      // Adjust scale if necessary
    bool swapRB = true;
    bool crop = false;

    cv::Mat blob = cv::dnn::blobFromImage(frame, scale, size, mean, swapRB, crop, CV_32F);
    net.setInput(blob);
    std::vector<cv::Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    std::vector<int> keep_classIds;
    std::vector<float> keep_confidences;
    std::vector<cv::Rect2d> keep_boxes;

    yoloPostProcessing(outs, keep_classIds, keep_confidences, keep_boxes);

    for (size_t idx = 0; idx < keep_boxes.size(); ++idx)
    {
        cv::Rect box = cv::Rect(keep_boxes[idx]);
        drawPrediction(keep_classIds[idx], keep_confidences[idx], box.x, box.y, box.width + box.x, box.height + box.y, frame);
    }
}

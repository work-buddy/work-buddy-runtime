#ifndef ORB_TRACKER_HPP
#define ORB_TRACKER_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

class ORBTracker
{
public:
    ORBTracker();
    ~ORBTracker();

    void processFrame(const cv::Mat &frame);
    void drawKeypoints(cv::Mat &output, const std::vector<cv::KeyPoint> &keypoints);
    std::vector<cv::KeyPoint> getKeypoints() const;
    cv::Mat getDescriptors() const;

private:
    void detectAndCompute(const cv::Mat &frameGray);
    void matchDescriptors();

    cv::Ptr<cv::ORB> orb;
    cv::BFMatcher matcher;
    cv::Mat prevFrameGray;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    std::vector<cv::KeyPoint> prevKeypoints;
    cv::Mat prevDescriptors;
};

#endif // ORB_TRACKER_HPP
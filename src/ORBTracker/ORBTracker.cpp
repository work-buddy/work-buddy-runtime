#include "ORBTracker.hpp"

ORBTracker::ORBTracker()
    : orb(cv::ORB::create()), matcher(cv::NORM_HAMMING) {}

ORBTracker::~ORBTracker() {}

void ORBTracker::processFrame(const cv::Mat &frame)
{
    cv::Mat frameGray;
    cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);

    if (!prevFrameGray.empty())
    {
        detectAndCompute(frameGray);
        matchDescriptors();
    }

    prevFrameGray = frameGray.clone();
    prevKeypoints = keypoints;
    prevDescriptors = descriptors.clone();
}

void ORBTracker::drawKeypoints(cv::Mat &output, const std::vector<cv::KeyPoint> &keypoints)
{
    cv::drawKeypoints(output, keypoints, output, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
}

std::vector<cv::KeyPoint> ORBTracker::getKeypoints() const
{
    return keypoints;
}

cv::Mat ORBTracker::getDescriptors() const
{
    return descriptors;
}

void ORBTracker::detectAndCompute(const cv::Mat &frameGray)
{
    orb->detectAndCompute(frameGray, cv::Mat(), keypoints, descriptors);
}

void ORBTracker::matchDescriptors()
{
    std::vector<cv::DMatch> matches;
    matcher.match(prevDescriptors, descriptors, matches);
}
#ifndef BASE64_HPP
#define BASE64_HPP

#include <opencv2/opencv.hpp>
#include <string>

std::string base64Encode(const cv::Mat &frame);

#endif // BASE64_HPP
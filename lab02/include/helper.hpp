//
// Created by Marco on 24/03/2021.
//

#ifndef LAB02_HELPER_H
#define LAB02_HELPER_H

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

int findCorners(const cv::Mat& img, std::vector<cv::Point2f>& corners, const cv::Size& size);
std::vector<cv::Point3f> createChessboard(const cv::Size& pattern_size, int side_length);
std::vector<cv::Point2f> createChessboard2D(const cv::Size& pattern_size, int side_length);

#endif //LAB02_HELPER_H

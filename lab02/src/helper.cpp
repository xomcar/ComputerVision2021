//
// Created by Marco on 24/03/2021.
//

#include "helper.hpp"

int findCorners(const cv::Mat& img, std::vector<cv::Point2f>& corners, const cv::Size& size) {
    bool res = cv::findChessboardCorners(img, size, corners,
                                         cv::CALIB_CB_ADAPTIVE_THRESH +
                                         cv::CALIB_CB_NORMALIZE_IMAGE +
                                         cv::CALIB_CB_FAST_CHECK);
    if (!res) {
        return 0;
    } else {
        cv::find4QuadCornerSubpix(img, corners, cv::Size(11,11));
        return corners.size();
    }
}

std::vector<cv::Point3f> createChessboard(const cv::Size& pattern_size, int side_length) {
    std::vector<cv::Point3f> points;
    for (int row = 0; row < pattern_size.height; row++) {
        for (int col = 0; col < pattern_size.width; col ++) {
            cv::Point3f p(static_cast<float>(row * side_length), static_cast<float>(col * side_length), 0);
            points.push_back(p);
        }
    }
    return points;
}

std::vector<cv::Point2f> createChessboard2D(const cv::Size& pattern_size, int side_length) {
    std::vector<cv::Point2f> points;
    for (int row = 0; row < pattern_size.height; row++) {
        for (int col = 0; col < pattern_size.width; col ++) {
            cv::Point2f p(static_cast<float>(row * side_length), static_cast<float>(col * side_length));
            points.push_back(p);}
    }
    return points;
}

//
// Created by Marco on 24/03/2021.
//

#include "helper.hpp"

int findCorners(cv::Mat& img, std::vector<cv::Point2f>& corners, cv::Size& size) {
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

std::vector<cv::Point3f> createChessboard(cv::Size& pattern_size, double side_length) {
    std::vector<cv::Point3f> points;
    for (int row = 0; row < pattern_size.height; row++) {
        for (int col = 0; col < pattern_size.width; col ++) {
            cv::Point3f p(static_cast<double>(row * side_length), static_cast<double>(col * side_length), 0);
            points.push_back(p);
        }
    }
    return points;
}

int argmin(const std::vector<double>& v) {
    return std::min_element(v.begin(), v.end()) - v.begin();
}

int argmax(const std::vector<double>& v) {
    return std::max_element(v.begin(), v.end()) - v.begin();
}

void printCameraMatrixInfo(const cv::Mat& camera) {

}

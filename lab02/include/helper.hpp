//
// Created by Marco on 24/03/2021.
//

#ifndef LAB02_HELPER_H
#define LAB02_HELPER_H

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

int findCorners(cv::Mat& img, std::vector<cv::Point2f>& corners, cv::Size& size);
std::vector<cv::Point3f> createChessboard(cv::Size& pattern_size, double side_length);
int argmin(const std::vector<double>& v);
int argmax(const std::vector<double>& v);
void printCameraMatrixInfo(const cv::Mat& camera);
void printDistCoeffInfo(const cv::Mat& coeffs);

#endif //LAB02_HELPER_H

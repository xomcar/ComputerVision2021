//
// Created by Marco on 07/04/2021.
//

#ifndef LAB03_HELPER_HIST_H
#define LAB03_HELPER_HIST_H

#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
cv::Mat plotHistogram(std::vector<cv::Mat>& hists, int color_space);
std::vector<cv::Mat> calcHistogram(cv::Mat& img, int color_space);
cv::Mat equalizeHistogramRGB(cv::Mat& src_img);
cv::Mat equalizeHistogramV(cv::Mat& src_img);
#endif //LAB03_HELPER_HIST_H

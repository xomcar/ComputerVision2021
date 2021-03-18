//
// Created by Marco on 18/03/2021.
//

#ifndef LAB01_HELPER_HPP
#define LAB01_HELPER_HPP

#endif //LAB01_HELPER_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

struct pixel {
    int x;
    int y;
    bool l_clicked;
    bool r_clicked;
};

typedef struct pixel bPixel;

int loadImageFromDisk(const std::string& path, const std::string& window_name, cv::Mat& img_buffer, float scale);
void leftClick(int event, int x, int y, int f, void* data);
bool checkBoundaries(int x, int y, int side, const cv::Mat& img);
cv::Vec3b getMean(int side, int x, int y, const cv::Mat& img);
cv::Mat thresholdingBRG(const cv::Vec3b& mean_color, const cv::Vec3b& new_color, const cv::Vec3b& threshold, const cv::Mat& src_img, int mask_size);
cv::Mat thresholdingH(const cv::Vec3b& mean_color, const cv::Vec3b& add_color, const cv::Vec3b& threshold, const cv::Mat& src_img);



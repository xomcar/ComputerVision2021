//
// Created by Marco on 18/03/2021.
//

#include "helper.hpp"

int loadImageFromDisk(const std::string& path, const std::string& window_name, cv::Mat& img_buffer, float scale) {
    img_buffer = cv::imread(path, cv::IMREAD_COLOR);
    if (img_buffer.empty()) {
        return -1;
    }
    cv::resizeWindow(window_name, img_buffer.cols * scale, img_buffer.rows * scale);
    return 0;
}

void leftClick(int event, int x, int y, int f, void* data) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        auto px = static_cast<bPixel*>(data);
        px->x = x;
        px->y = y;
        px->l_clicked = true;
    }
    if (event == cv::EVENT_RBUTTONDOWN) {
        auto px = static_cast<bPixel*>(data);
        px->x = x;
        px->y = y;
        px->r_clicked = true;
    }
}

bool checkBoundaries(int x, int y, int side, const cv::Mat& img) {
    if (x < side || x > img.cols - side) return false;
    if (y < side || y > img.rows - side) return false;
    return true;
}

cv::Vec3b getMean(int side, int x, int y, const cv::Mat& img) {
    cv::Mat window = cv::Mat(img, cv::Rect(x + side / 2, y + side / 2, side, side));
    int b_value = 0, g_value = 0, r_value = 0;
    int n = side * side;
    for (int r = 0; r < side; r++) {
        for (int c = 0; c < side; c++) {
            b_value += window.at<cv::Vec3b>(r, c)[0];
            g_value += window.at<cv::Vec3b>(r, c)[1];
            r_value += window.at<cv::Vec3b>(r, c)[2];
        }
    }
    cv::Vec3b mean = cv::Vec3b(b_value / n, g_value / n, r_value / n);
    return mean;
}

cv::Mat thresholdingBRG(const cv::Vec3b& mean_color, const cv::Vec3b& new_color, const cv::Vec3b& threshold, const cv::Mat& src_img, int masksize) {
    cv::Mat out_img = src_img.clone();
    cv::Mat mask;
    cv::Scalar min = cv::Scalar(mean_color[0] - threshold[0], mean_color[1] - threshold[1], mean_color[2] - threshold[2]);
    cv::Scalar max = cv::Scalar(mean_color[0] + threshold[0], mean_color[1] + threshold[1], mean_color[2] + threshold[2]);
    cv::inRange(src_img, min, max, mask);
    int nonzero = cv::countNonZero(mask);
    if (nonzero < 0.3 * masksize || nonzero > masksize) return cv::Mat{};
    cv::Mat area = cv::Mat(src_img.size(), src_img.type(), cv::Scalar(new_color));
    cv::add(area, out_img, out_img, mask);
    return out_img;
}

cv::Mat thresholdingH(const cv::Vec3b& mean_color, const cv::Vec3b& add_color, const cv::Vec3b& threshold, const cv::Mat& src_img) {
    cv::Mat out_img = src_img.clone();
    cv::Mat mask;
    cv::Scalar min = cv::Scalar(mean_color[0] - threshold[0], mean_color[1] - threshold[1], mean_color[2] - threshold[2]);
    cv::Scalar max = cv::Scalar(mean_color[0] + threshold[0], mean_color[1] + threshold[1], mean_color[2] + threshold[2]);
    cv::inRange(src_img, min, max, mask);
    for (int r = 0; r < mask.rows; r++) {
        for (int c = 0; c < mask.cols; c++) {
            if (mask.at<bool>(r, c))
                out_img.at<cv::Vec3b>(r, c) += add_color;
        }
    }
    return out_img;
}




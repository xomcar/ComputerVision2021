//
// Created by Marco on 07/04/2021.
//
#include <iostream>
#include "helper_hist.h"

#define RGB_COLOR_SPACE 0
#define HSV_COLOR_SPACE 1

int main() {
    std::vector<cv::String> filenames;
    cv::glob("*.jpg", filenames);
    if (filenames.empty()) {
        std::cout << "No images found. Exiting...\n";
        return 1;
    }
    std::cout << "Images found:\n";
    int idx = 0;
    for (const auto& name : filenames) {
        std::cout << "\t[" << idx++ << "] " << name << std::endl;
    }
    std::cout << "Select image to filter:";
    int sel;
    while (1) {
        std::cin >> sel;
        if (!std::cin) {
            std::cout << "Invalid input. Insert an integer value!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        } else if (sel < 0 || sel > idx) {
            std::cout << "Insert a value within the listed ones!\n";
        }
        else {
            std::cout << "Selected " << filenames[sel] << std::endl;
            break;
        }
    }

    auto src_img_rgb = cv::imread(filenames[sel]);
    cv::Mat src_img_hsv;
    cv::cvtColor(src_img_rgb, src_img_hsv, cv::COLOR_BGR2HSV);

    // RGB manipulation
    auto rgb_og_histograms = calcHistogram(src_img_rgb, RGB_COLOR_SPACE);
    auto rgb_og_result = plotHistogram(rgb_og_histograms, RGB_COLOR_SPACE);
    auto rgb_eq_img = equalizeHistogramRGB(src_img_rgb);
    auto rgb_eq_histograms = calcHistogram(rgb_eq_img, RGB_COLOR_SPACE);
    auto rgb_eq_result = plotHistogram(rgb_eq_histograms, RGB_COLOR_SPACE);
    cv::Mat rgb_hist_comparison;
    cv::vconcat(rgb_og_result, rgb_eq_result, rgb_hist_comparison);
    auto rgb_hist_win = "RGB histograms comparison - Upper: original | Lower: equalized";
    cv::imshow(rgb_hist_win, rgb_hist_comparison);

    // HSV manipulation
    auto hsv_og_histograms = calcHistogram(src_img_hsv, HSV_COLOR_SPACE);
    auto hsv_og_result = plotHistogram(hsv_og_histograms, HSV_COLOR_SPACE);
    auto hsv_eq_img = equalizeHistogramV(src_img_hsv);
    auto hsv_eq_histograms = calcHistogram(hsv_eq_img, HSV_COLOR_SPACE);
    auto hsv_eq_result = plotHistogram(hsv_eq_histograms, HSV_COLOR_SPACE);
    cv::cvtColor(hsv_eq_img, hsv_eq_img, cv::COLOR_HSV2BGR);
    cv::Mat hsv_hist_comparison;
    cv::vconcat(hsv_og_result, hsv_eq_result, hsv_hist_comparison);
    auto hsv_hist_win = "HSV histograms comparison - Upper: original | Lower: equalized";
    cv::imshow(hsv_hist_win, hsv_hist_comparison);

    // Evaluating results
    cv::Mat image_comparison;
    cv::hconcat(src_img_rgb, rgb_eq_img, image_comparison);
    cv::hconcat(image_comparison, hsv_eq_img, image_comparison);
    auto comparison = "Image comparison - Left: original | Middle: RGB equalized | Right: HSV (V only) equalized";
    cv::namedWindow(comparison, cv::WINDOW_NORMAL);
    cv::resizeWindow(comparison, cv::Size(0.5 * image_comparison.cols, 0.5 * image_comparison.rows));
    cv::imshow(comparison, image_comparison);

    // Close watchdog
    std::cout << "Close any window to exit...\n";
    while(true) {
        cv::waitKey(1000);
        if (cv::getWindowProperty(comparison, cv::WND_PROP_AUTOSIZE) == -1 ||
            cv::getWindowProperty(hsv_hist_win, cv::WND_PROP_AUTOSIZE) == -1 ||
            cv::getWindowProperty(rgb_hist_win, cv::WND_PROP_AUTOSIZE) == -1) {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }
    }
}
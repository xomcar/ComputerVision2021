//
// Created by Marco on 27/04/2021.
//

#include "helper.h"
#include <iostream>

int main(int argc, char** args) {
    if (argc == 1) {
        std::cout << "Usage: executable image.jpg\n";
        return 1;
    }
    cv::Mat img = cv::imread(args[1]);
    if (img.empty()) {
        std::cout << "Invalid image file\n";
        return 1;
    }
    auto win_n = "Test";
    Detector d(img, win_n);
    d.startCannyWindow();
    while (true) {
        cv::waitKey(100);
        d.updateWindow();
        if (static_cast<bool>(cv::getWindowProperty(win_n, cv::WindowPropertyFlags::WND_PROP_AUTOSIZE == -1))) {
            d.closeWindow();
            break;
        }
    }
    d.startLinesWindow();
    while (true) {
        cv::waitKey(100);
        d.updateWindow();
        if (static_cast<bool>(cv::getWindowProperty(win_n, cv::WindowPropertyFlags::WND_PROP_AUTOSIZE == -1))) {
            d.closeWindow();
            break;
        }
    }
    d.startCirclesWindow();
    while (true) {
        cv::waitKey(100);
        d.updateWindow();
        if (static_cast<bool>(cv::getWindowProperty(win_n, cv::WindowPropertyFlags::WND_PROP_AUTOSIZE == -1))) {
            d.closeWindow();
            break;
        }
    }
    auto out = d.getFinalResult();
    cv::imshow(win_n, out);
    while (true) {
        cv::waitKey(100);
        if (static_cast<bool>(cv::getWindowProperty(win_n, cv::WindowPropertyFlags::WND_PROP_AUTOSIZE == -1))) {
            break;
        }
    }
    return 0;
}
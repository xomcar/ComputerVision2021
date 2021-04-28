//
// Created by Marco on 27/04/2021.
//

#include "helper.h"

int main() {
    auto img = cv::imread("input.png");
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
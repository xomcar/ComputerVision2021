//
// Created by Marco on 04/05/2021.
//
#include <opencv2/core.hpp>
#include "ObjectTracker_bad.h"
#include <iostream>

int main() {
    cv::Mat curr_frame;
    cv::VideoCapture captureObj;
    std::vector<cv::Mat> objects;
    std::vector<cv::String> filenames;
    cv::glob("camera/*.jpg", filenames, false);
    std::vector<ObjectTracker_bad> trackers;

    captureObj.open(1, 0);
    if (!captureObj.isOpened()) {
        std::cerr << "Camera stream failed.\n";
        return 1;
    }
    while (true) {
        captureObj.read(curr_frame);
        if (curr_frame.empty()) {
            std::cerr << "Blank frame!\n";
            return 1;
        }
        cv::imshow("Press a button to match", curr_frame);
        if (cv::waitKey(50) >= 0) {
            break;
        }
    }
    std::vector<cv::KeyPoint> frame_keys;
    cv::Mat frame_desc;
    auto frame_det = cv::SIFT::create();
    frame_det->detectAndCompute(curr_frame, cv::noArray(), frame_keys, frame_desc);

    for (const auto& file : filenames) {
        cv::Mat curr_img = cv::imread(file, cv::IMREAD_COLOR);
        if (curr_img.empty()) {
            std::cerr << "Failed to load image " << file << std::endl;
            return 1;
        }
        objects.push_back(curr_img.clone());
        trackers.emplace_back(curr_img, &frame_desc, &frame_keys, 0.6);
        std::cout << "Loaded image " << file << " as tracking object" << std::endl;
    }

    int idx = 0;
    cv::Mat display = curr_frame.clone();
    for (auto& tracker : trackers) {
        std::cout << "Computing matches for " << filenames[idx++] << std::endl;
        display = tracker.drawRectangle(display);

    }
    cv::namedWindow("Detected", cv::WINDOW_NORMAL);
    cv::resizeWindow("Detected", 1280, 720);
    cv::imshow("Detected", display);
    cv::waitKey();
    cv::destroyAllWindows();
    captureObj.release();
    return 0;
}


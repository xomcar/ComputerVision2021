//
// Created by Marco on 04/05/2021.
//

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <ObjectTracker.h>

int main(int argc, char** argv) {
    cv::Mat curr_frame;
    cv::VideoCapture captureObj("video.mov");
    std::vector<cv::String> filenames;
    cv::glob("objects/*.png", filenames, false);
    std::vector<cv::Mat> objects;
    std::vector<ObjectTracker> trackers;

    captureObj.read(curr_frame);
    if (curr_frame.empty()) {
        std::cerr << "Blank frame_bw grabbed\n";
        return 1;
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
        trackers.emplace_back(curr_img, &frame_desc, &frame_keys, 0.5);
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
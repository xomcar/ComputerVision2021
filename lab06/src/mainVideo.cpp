//
// Created by Marco on 04/05/2021.
//

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <ObjectTracker.h>

int main(int argc, char** argv) {
    cv::RNG rng(124);
    cv::Mat curr_frame;
    cv::VideoCapture captureObj("video.mp4");
    std::vector<cv::String> filenames;
    cv::glob("objects/*.png", filenames, false);
    std::vector<ObjectTracker> trackers;

    // Read first frame of video
    captureObj.read(curr_frame);
    if (curr_frame.empty()) {
        std::cerr << "Blank frame_bw grabbed\n";
        return 1;
    }
    cv::Mat curr_frame_bw;
    cv::cvtColor(curr_frame, curr_frame_bw, cv::COLOR_RGB2GRAY);

    // Detect keypoints on frame
    std::vector<cv::KeyPoint> frame_keys;
    cv::Mat frame_desc;
    auto frame_det = cv::SIFT::create();
    frame_det->detectAndCompute(curr_frame, cv::noArray(), frame_keys, frame_desc);

    // Create trackers for each object
    for (const auto& file : filenames) {
        // Read image in bw
        cv::Mat obj_bw = cv::imread(file, cv::IMREAD_GRAYSCALE);
        if (obj_bw.empty()) {
            std::cerr << "Failed to load image " << file << std::endl;
            return 1;
        }
        // Create random color
        auto color = cv::Scalar(rng.uniform(0,255),
                                rng.uniform(0, 255), rng.uniform(0, 255));
        // Create tracker for object
        trackers.emplace_back(obj_bw, curr_frame_bw, &frame_desc, &frame_keys, color);
        std::cout << "Loaded image " << file << ", now detecting..." << std::endl;
    }

    cv::Mat display = curr_frame.clone();
    for (auto& tracker : trackers) {
        tracker.draw(display);
    }
    cv::namedWindow("Detected", cv::WINDOW_NORMAL);
    cv::resizeWindow("Detected", 1280, 720);
    cv::imshow("Detected", display);
    cv::waitKey();
    cv::destroyAllWindows();

    captureObj.release();
    return 0;
}
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
                                rng.uniform(0, 255),
                                rng.uniform(0, 255));
        // Create tracker for object
        std::cout << "Loaded image " << file << std::endl;
        trackers.emplace_back(curr_frame_bw, obj_bw, 0.3, color);
    }


    cv::Mat display;
    // Video feed scan
    cv::Mat next_frame, next_frame_bw;
    while (true) {
        captureObj.read(next_frame);
        if (next_frame.empty())
            break;
        display = next_frame.clone();
        cv::cvtColor(next_frame, next_frame_bw, cv::COLOR_BGR2GRAY);
        for (auto& tracker : trackers) {
            tracker.track(next_frame_bw, display);
        }
        cv::imshow("Feed", display);
        if (cv::waitKey(5) >= 0)
            break;
    }

    // Close video capture
    captureObj.release();
    return 0;
}
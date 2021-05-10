//
// Created by Marco on 04/05/2021.
//

#ifndef LAB06_OBJECTTRACKER_H
#define LAB06_OBJECTTRACKER_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

class ObjectTracker {
private:
    // Private parameters
    // Black and white image of object to track
    cv::Mat obj_bw;
    // Black and white images of track frame
    cv::Mat frame_bw;
    // Homography matrix to compute translation
    cv::Mat H;
    // Mask of matches for RANSAC
    std::vector<uint8_t> match_mask;
    // Color to assign to object
    cv::Scalar color;
    // Vector of keypoints in both obj image and track frame
    std::vector<cv::KeyPoint> obj_keys, obj_keys_frame;
    // Pointer to vector of keypoints in track frame (computed externally)
    std::vector<cv::KeyPoint>* ext_frame_keys;
    // Vector of descriptors for keypoints
    cv::Mat obj_desc;
    // Pointer to vector of descriptors in track frame (computed externally)
    cv::Mat* ext_frame_desc;
    // Vector of matches
    std::vector<cv::DMatch> matches;
    // Vectors for storing pixel locations of keypoints
    std::vector<cv::Point2f> frame_points, obj_points;
    // Vector storing coordinates of rectangle vertices
    std::vector<cv::Point2f> rec = std::vector<cv::Point2f>(4);
    // Matcher and detector objects
    cv::Ptr<cv::SiftFeatureDetector> detector;
    cv::Ptr<cv::BFMatcher> matcher;
    // Ratio of exclusion in match distance
    float ratio = 0.3;

    // Now for private functions
    // Detect and match keypoints
    void detectAndMatch();
    // Compute matches sanitization using RANSAC
    void purgeMatches();
    // Draw keypoints on an image
    void drawKeypoints(cv::Mat& image);
    // Draw rectangle on an image
    void drawRectangle(cv::Mat& image);
    // Execute initialization
    void initialize();

public:
    // Constructor
    ObjectTracker(const cv::Mat& object_bw, cv::Mat& frame_bw, cv::Mat* frame_descriptors,
                  std::vector<cv::KeyPoint>* frame_keypoints, cv::Scalar& color);
    // Draws current keypoints and rectangle on an image
    void draw(cv::Mat& image);
    // Track movement using optical flow
    void track(cv::Mat& next_frame);

};


#endif //LAB06_OBJECTTRACKER_H

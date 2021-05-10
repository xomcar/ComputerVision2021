//
// Created by marco on 5/10/2021.
//

#include "ObjectTracker.h"

ObjectTracker::ObjectTracker(const cv::Mat& object_bw, cv::Mat& frame_bw, cv::Mat* frame_descriptors,
                             std::vector<cv::KeyPoint>* frame_keypoints, cv::Scalar& col) {
    obj_bw = object_bw;
    frame_bw = frame_bw;
    color = col;
    ext_frame_desc = frame_descriptors;
    ext_frame_keys = frame_keypoints;
    // Initialize rectangle coordinates
    rec[0] = cv::Point2f(0, 0);
    rec[1] = cv::Point2f(0, obj_bw.rows);
    rec[2] = cv::Point2f(obj_bw.cols, obj_bw.rows);
    rec[3] = cv::Point2f(obj_bw.cols, 0);
    detector = cv::SIFT::create();
    matcher = cv::BFMatcher::create();
    initialize();
}

void ObjectTracker::detectAndMatch() {
    // Compute detection and matching
    detector->detectAndCompute(obj_bw, cv::noArray(), obj_keys, obj_desc);
    matcher->match(*ext_frame_desc, obj_desc, matches, cv::noArray());
}

void ObjectTracker::purgeMatches() {
    // Discard matches according to distance
    // Sort ascending matches
    std::sort(matches.begin(), matches.end());
    // Save max distance
    float max_dist = matches.back().distance;
    // If a match is over the threshold, pop it out
    while (max_dist * ratio < matches.back().distance) {
        matches.pop_back();
    }
    // Now save pixel coordinates of points
    // Pick each match
    for (auto match : matches) {
        // Index of matched keypoint in frame keypoints
        auto frame_idx = match.queryIdx;
        // Index of matched keypoint in object keypoints
        auto obj_idx = match.trainIdx;
        // Save keypoints location
        frame_points.push_back(ext_frame_keys->at(frame_idx).pt);
        obj_points.push_back(obj_keys[obj_idx].pt);
    }
    // Compute homography and cleanse using RANSAC
    // Compute homography
    H = cv::findHomography(obj_points, frame_points,
                           cv::RANSAC, 4, match_mask);
    int i = 0;
    // Empty the keypoints location
    frame_points.clear();
    obj_points.clear();
    // Pick each match
    for (auto match : matches) {
        // If the match is not good for the RANSAC mask, skip
        if (!match_mask[i++]) continue;
        // Save both
        auto frame_idx = match.queryIdx;
        auto obj_idx = match.trainIdx;
        frame_points.push_back(ext_frame_keys->at(frame_idx).pt);
        obj_points.push_back(obj_keys[obj_idx].pt);
        // Also save matched keypoints in track frame (for drawing)
        obj_keys_frame.push_back(ext_frame_keys->at(frame_idx));
    }
    // Move coordinates of rectangle according to homography
    cv::perspectiveTransform(rec, rec, H);
}

void ObjectTracker::drawKeypoints(cv::Mat &image) {
    cv::drawKeypoints(frame_bw, obj_keys_frame,
                      image, color, cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
}

void ObjectTracker::drawRectangle(cv::Mat &image) {
    for (int i = 0; i < 3; i++) {
        cv::line(image, rec[i], rec[i + 1], color, 3);
    }
    cv::line(image, rec[3], rec[0], color, 3);
}

void ObjectTracker::initialize() {
    detectAndMatch();
    purgeMatches();
}

void ObjectTracker::draw(cv::Mat &image) {
    drawKeypoints(image);
    drawRectangle(image);
}

void ObjectTracker::track(cv::Mat &next_frame) {
    std::vector<uchar> status;
    std::vector<cv::Point2f> next_frame_points;
    cv::Mat next_bw;
    cv::cvtColor(next_frame, next_bw, cv::COLOR_BGR2GRAY);
    // Compute optical flow
    cv::calcOpticalFlowPyrLK(frame_bw, next_bw, frame_points,
                             next_frame_points, status, cv::noArray());
    // Compute affine transformation between frames
    cv::Mat T = cv::estimateAffine2D(frame_points, next_frame_points);
    // Move rectangle
    for (auto &vertex : rec) {
        vertex.x += T.at<double>(0,2);
        vertex.y += T.at<double>(1, 2);
    }
    // Move keypoints
    for (auto &keypoint : obj_keys_frame) {
        keypoint.pt.x += T.at<double>(0,2);
        keypoint.pt.y += T.at<double>(1,2);
    }
    // Save the newfound points as current points
    std::swap(frame_points, next_frame_points);
    // Save new frame as current
    frame_bw = next_bw;
    // Draw keypoints and rectangles on next frame
    draw(next_frame);
}
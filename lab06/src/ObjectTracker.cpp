//
// Created by Marco on 04/05/2021.
//

#include "ObjectTracker.h"
ObjectTracker::ObjectTracker(const cv::Mat& og_img, cv::Mat* frame_desc,
                             std::vector<cv::KeyPoint>* frame_keys, float ratio) {
    this->ratio = ratio;
    this->object_rgb = og_img.clone();
    cv::cvtColor(this->object_rgb, this->object_bw, cv::COLOR_BGR2GRAY);
    this->frame_keys = frame_keys;
    this->frame_desc = frame_desc;
    this->rng = cv::RNG(cv::getTickCount());
    this->color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
    detector = cv::SIFT::create();
    matcher = cv::BFMatcher::create(cv::NORM_L2, false);
}

void ObjectTracker::detect() {
    if (detected) {
        obj_keys.clear();
        obj_desc.release();
        matches.clear();
    }
    detector->detectAndCompute(object_bw, cv::noArray(), obj_keys, obj_desc);
    matcher->match(*frame_desc, obj_desc, matches, cv::noArray());
    purgeMatches();
    findHomography();
    detected = true;
}

void ObjectTracker::purgeMatches() {
    std::sort(matches.begin(), matches.end());
    max_dist = matches.back().distance;
    while (max_dist * ratio < matches.back().distance) {
        matches.pop_back();
    }
}

void ObjectTracker::findHomography() {
    for (auto match : matches) {
        auto frame_idx = match.queryIdx;
        auto obj_idx = match.trainIdx;
        frame_points.push_back(frame_keys->at(frame_idx).pt);
        obj_points.push_back(obj_keys[obj_idx].pt);
    }
    H = cv::findHomography(obj_points, frame_points, cv::RANSAC, 4, match_mask);
}

cv::Mat ObjectTracker::drawKeypoints(const cv::Mat& frame_img) {
    if (!detected) detect();
    cv::drawMatches(frame_img, *frame_keys, object_rgb, obj_keys, matches,
                    out_img, color, cv::Scalar::all(-1), match_mask,
                    cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    return out_img;
}

cv::Mat ObjectTracker::drawRectangle(const cv::Mat& frame_img) {
    if (!detected) detect();
    cv::Mat temp = frame_img.clone();
    std::vector<cv::Point2f> rec;
    rec.emplace_back(0, 0);
    rec.emplace_back(object_bw.cols, 0);
    rec.emplace_back(object_bw.cols, object_bw.rows);
    rec.emplace_back(0, object_bw.rows);
    cv::perspectiveTransform(rec, rec, H);
    for (int i = 0; i < rec.size() - 1; i++) {
        cv::line(temp, rec[i], rec[i+1], color, 3);
    }
    cv::line(temp, rec[3], rec[0], color, 3);
    return temp;
}



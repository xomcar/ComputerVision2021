//
// Created by marco on 5/10/2021.
//

#include "ObjectTracker.h"

ObjectTracker::ObjectTracker(const cv::Mat& frame_bw, const cv::Mat& object_bw,
                             const float& rat, const cv::Scalar& col) {
    obj_bw = object_bw;
    curr_bw = frame_bw;
    color = col;
    rec[0] = cv::Point2f(0, 0);
    rec[1] = cv::Point2f(0, obj_bw.rows);
    rec[2] = cv::Point2f(obj_bw.cols, obj_bw.rows);
    rec[3] = cv::Point2f(obj_bw.cols, 0);
    detector = cv::SIFT::create();
    matcher = cv::BFMatcher::create();
    ratio = rat;
}

void ObjectTracker::track(const cv::Mat &next_frame_bw, cv::Mat &output_image) {
    if (need_detect) {
        std::cout << "Computing detection..." << std::endl;
        detectAndMatch();
        purgeMatches();
        need_detect = false;
        features = key_loc_curr.size();
        std::cout << "Detected " << features << std::endl;
    } else {
        next_bw = next_frame_bw.clone();
        key_loc_next.clear();
        cv::calcOpticalFlowPyrLK(curr_bw, next_bw, key_loc_curr,
                                 key_loc_next, status, cv::noArray());
        std::vector<cv::Point2f> curr_good, next_good;
        int i = 0;
        for (auto& point : key_loc_curr) {
            if (status[i] == 1) {
                curr_good.push_back(key_loc_curr[i]);
                next_good.push_back(key_loc_next[i]);
            }
            i++;
        }
        if (curr_good.size() < features / 2) {
            need_detect = true;
            std::cout << "Need refinement, " << curr_good.size() << "/" << features << std::endl;
        }

        T = cv::estimateAffine2D(curr_good, next_good);
        for (auto& ver : rec) {
            cv::Vec3d pnt = {ver.x, ver.y, 1};
            cv::Mat out = T * pnt;
            ver.x = static_cast<float>(out.at<double>(0));
            ver.y = static_cast<float>(out.at<double>(1));
        }
        key_loc_curr = std::move(next_good);
    }
    curr_bw = next_frame_bw.clone();
    drawKeypoints(output_image);
    drawRectangle(output_image);
}

void ObjectTracker::detectAndMatch() {
    key_pnt_obj.clear();
    key_pnt_curr.clear();
    curr_frame_desc.release();
    obj_desc.release();
    matches.clear();
    detector->detectAndCompute(obj_bw, cv::noArray(), key_pnt_obj,
                               obj_desc);
    detector->detectAndCompute(curr_bw, cv::noArray(), key_pnt_curr,
                               curr_frame_desc);
    matcher->match(curr_frame_desc, obj_desc, matches);
}

void ObjectTracker::purgeMatches() {
    std::sort(matches.begin(), matches.end());
    auto max_dist = matches.back().distance;
    while (max_dist * ratio < matches.back().distance) {
        matches.pop_back();
    }
    for (auto& match : matches) {
        key_loc_obj.push_back(key_pnt_obj[match.trainIdx].pt);
        key_loc_curr.push_back(key_pnt_curr[match.queryIdx].pt);
    }
    H = cv::findHomography(key_loc_obj, key_loc_curr,
                           cv::RANSAC, 4, mask);
    key_loc_obj.clear();
    key_loc_curr.clear();
    int i = 0;
    for (auto& match : matches) {
        if (mask[i++] == 1) {
            key_loc_obj.push_back(key_pnt_obj[match.trainIdx].pt);
            key_loc_curr.push_back(key_pnt_curr[match.queryIdx].pt);
        }
    }
    cv::perspectiveTransform(rec, rec, H);
}

void ObjectTracker::drawKeypoints(cv::Mat& image) {
    for (const auto& point : key_loc_curr) {
        cv::circle(image, point, 4, color, 2);
    }
}

void ObjectTracker::drawRectangle(cv::Mat &image) {
    cv::line(image, rec[0], rec[1], color, 3);
    cv::line(image, rec[1], rec[2], color, 3);
    cv::line(image, rec[2], rec[3], color, 3);
    cv::line(image, rec[3], rec[0], color, 3);
}

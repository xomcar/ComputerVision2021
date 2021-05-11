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
    cv::Mat obj_bw, curr_bw, next_bw;
    cv::Mat obj_desc, curr_frame_desc;
    std::vector<cv::Point2f> key_loc_obj, key_loc_curr, key_loc_next;
    std::vector<cv::KeyPoint> key_pnt_obj, key_pnt_curr;
    std::vector<cv::DMatch> matches;
    bool need_detect = true;
    cv::Mat H, T;
    cv::Ptr<cv::SIFT> detector;
    cv::Ptr<cv::BFMatcher> matcher;
    float ratio;
    size_t features = 0;
    cv::Scalar color;
    std::vector<uchar> mask, status;
    std::vector<cv::Point2f> rec = std::vector<cv::Point2f>(4);

    void detectAndMatch();
    void purgeMatches();
    void drawKeypoints(cv::Mat& image);
    void drawRectangle(cv::Mat& image);
public:
    ObjectTracker(const cv::Mat &frame_bw, const cv::Mat &object_bw,
                  const float &ratio, const cv::Scalar& col);
    void track(const cv::Mat &next_frame_bw, cv::Mat &output_image);
};

#endif //LAB06_OBJECTTRACKER_H

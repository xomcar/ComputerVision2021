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

class ObjectTracker {
private:
    cv::RNG rng;
    cv::Mat object_rgb, object_bw, out_img;
    std::vector<cv::KeyPoint> obj_keys;
    cv::Mat obj_desc, H;
    cv::Scalar color;
    std::vector<cv::KeyPoint>* frame_keys;
    cv::Mat* frame_desc;
    std::vector<cv::DMatch> matches;
    std::vector<cv::Point2f> obj_points, frame_points;
    cv::Ptr<cv::SiftFeatureDetector> detector;
    cv::Ptr<cv::BFMatcher> matcher;
    std::vector<char> match_mask;
    bool detected = false;
    float max_dist;
    float ratio;

    void purgeMatches();
    void findHomography();
public:
    ObjectTracker(const cv::Mat& og_img, cv::Mat* frame_desc,
                  std::vector<cv::KeyPoint>* frame_keys, float ratio);
    void detect();
    cv::Mat drawKeypoints(const cv::Mat& frame_img);
    cv::Mat drawRectangle(const cv::Mat& frame_img);
};


#endif //LAB06_OBJECTTRACKER_H

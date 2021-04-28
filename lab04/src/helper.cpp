//
// Created by Marco on 27/04/2021.
//

#include "helper.h"
#include <iostream>
#include <utility>

Detector::Detector(cv::Mat& image, cv::String name) {
    og_img_color = image.clone();
    cv::cvtColor(image, og_img_bw, cv::COLOR_BGR2GRAY);
    canny_img = og_img_bw.clone();
    temp_img = og_img_color.clone();
    lines_img = og_img_color.clone();
    circles_img = og_img_color.clone();
    final_img = og_img_color.clone();
    win_name = std::move(name);
}

void Detector::closeWindow() {
    win_open = false;
}

void Detector::startCannyWindow() {
    win_open = true;
    cv::namedWindow(win_name);
    cv::createTrackbar(blur_bar, win_name, NULL, max_blur, onChangeBlur, this);
    cv::createTrackbar(T1_bar, win_name, NULL, max_T1, onChangeT1, this);
    cv::createTrackbar(T2_bar, win_name, NULL, max_T2, onChangeT2, this);
    cv::createTrackbar(sobel_bar, win_name, NULL, max_sobel, onChangeSobel, this);
    cv::imshow(win_name, canny_img);
}

void Detector::startLinesWindow() {
    win_open = true;
    cv::namedWindow(win_name);
    cv::createTrackbar(rho_bar, win_name, NULL, max_rho, onChangeRho, this);
    cv::createTrackbar(theta_bar, win_name, NULL, max_theta, onChangeTheta, this);
    cv::createTrackbar(threshold_bar, win_name, NULL, max_threshold, onChangeThreshold, this);
    cv::setTrackbarPos(threshold_bar, win_name, 128);
    cv::imshow(win_name, lines_img);
}

void Detector::startCirclesWindow() {
    win_open = true;
    cv::namedWindow(win_name);
    cv::createTrackbar(accumulator_bar, win_name, NULL, max_acc, onChangeAcc, this);
    cv::createTrackbar(min_dist_bar, win_name, NULL, max_min_dist, onChangeMinDist, this);
    cv::createTrackbar(param1_bar, win_name, NULL, max_par1, onChangeParam1, this);
    cv::createTrackbar(param2_bar, win_name, NULL, max_par2, onChangeParam2, this);
    cv::setTrackbarPos(accumulator_bar, win_name, 1);
    cv::imshow(win_name, circles_img);
}

void Detector::updateWindow() {
    if (!to_update || !win_open) return;
    compute();
    switch (t) {
        case canny:
            cv::imshow(win_name, canny_img);
            break;
        case hlines:
            cv::imshow(win_name, lines_img);
            break;
        case hcircles:
            cv::imshow(win_name, circles_img);
            break;
    }
    to_update = false;
}

void Detector::compute() {
    if (canny_param_changed) {
        cv::blur(og_img_bw.clone(), temp_img, cv::Size(blur, blur));
        cv::Canny(temp_img, canny_img, T1, T2, sobel);
        t = canny;
        canny_param_changed = false;
    } else if (lines_param_changed) {
        lines.clear();
        lines_img = og_img_color.clone();
        cv::HoughLines(canny_img.clone(), lines, rho, theta, threshold);
        for (auto line : lines) {
            drawLine(lines_img, line[0], line[1], 1000, cv::Scalar(0, 0, 255));
        }
        if (lines.size() == 2) {
            left_2_lines = true;
            lines_img = og_img_color.clone();
            drawStreet();
        } else {
            left_2_lines = false;
        }
        t = hlines;
        lines_param_changed = false;
    } else if (circle_param_changed) {
        circles.clear();
        circles_img = og_img_color.clone();
        GaussianBlur(og_img_bw.clone(), temp_img, cv::Size(5, 5), 1.5, 1.5);
        cv::HoughCircles(temp_img.clone(), circles, cv::HOUGH_GRADIENT, accumulator,
                         min_dist, param1, param2, 2, 100);
        std::cout << "MinD:" << min_dist << " Dp:" << accumulator <<
        " P1:" << param1 << " P2:" << param2 << " Circles:" << circles.size() << std::endl;
        for (auto circle : circles) {
            drawCircle(circles_img, cv::Point(cvRound(circle[0]),cvRound(circle[1])),
                       circle[2], cv::Scalar(0,255,0));
        }
        if (circles.size() == 1) {
            left_1_circle = true;
            circles_img = og_img_color.clone();
            drawSign();
        } else {
            left_1_circle = false;
        }
        t = hcircles;
        lines_param_changed = false;
    }
}

void Detector::drawStreet() {
    if (!left_2_lines) {
        std::cout << "More than 2 lines left, try again with different values." << std::endl;
    }
    auto bottom = cv::Vec3f(og_img_color.rows, 90 * deg2rad, 0);
    std::vector<cv::Point> vertices;
    vertices.push_back(findIntersect(lines[0], lines[1]));
    vertices.push_back(findIntersect(lines[0], bottom));
    vertices.push_back(findIntersect(lines[1], bottom));
    cv::fillConvexPoly(lines_img, vertices, cv::Scalar(0, 0, 255));
}

void Detector::drawSign() {
    if (!left_1_circle) {
        std::cout << "More than 1 circle is left, try again with different values." << std::endl;
    }
    circle(circles_img, cv::Point(cvRound(circles[0][0]), cvRound(circles[0][1])),
           circles[0][2], cv::Scalar(0, 255, 0), -1, 8, 0);
}

cv::Mat Detector::getFinalResult() {
    if (left_1_circle && left_2_lines) {
        auto bottom = cv::Vec3f(og_img_color.rows, 90 * deg2rad, 0);
        std::vector<cv::Point> vertices;
        vertices.push_back(findIntersect(lines[0], lines[1]));
        vertices.push_back(findIntersect(lines[0], bottom));
        vertices.push_back(findIntersect(lines[1], bottom));
        cv::fillConvexPoly(final_img, vertices, cv::Scalar(0, 0, 255));
        circle(final_img, cv::Point(cvRound(circles[0][0]), cvRound(circles[0][1])),
               circles[0][2], cv::Scalar(0, 255, 0), -1, 8, 0);
        return final_img;
    }
    return og_img_color;
}

// Utility functions
void Detector::drawLine(cv::Mat& img, double rho, double theta, double delta, cv::Scalar color) {
    cv::Point ps[2];
    getPoints(rho, theta, ps, delta);
    cv::line(img, ps[0], ps[1], color, 3, cv::LINE_AA);
}
void Detector::drawCircle(cv::Mat &img, cv::Point center, double radius, cv::Scalar color) {
    circle( img, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
    circle( img, center, radius, color, 3, 8, 0 );
}
cv::Vec2f Detector::getParam(cv::Point p1, cv::Point p2) {
    cv::Vec2f params;
    params[0] = (float)(p2.y - p1.y) / (float)(p2.x - p1.x); //m
    params[1] = (float)p1.y - (float)params[0] * p1.x; //b
    return params;
}
cv::Point Detector::findIntersect(cv::Vec3f line1, cv::Vec3f line2) {
    cv::Point ps1[2], ps2[2];
    getPoints(line1[0], line1[1], ps1, 50);
    getPoints(line2[0], line2[1], ps2, 50);
    auto params1 = getParam(ps1[0], ps1[1]);
    auto params2 = getParam(ps2[0], ps2[1]);
    auto m1 = params1[0], m2 = params2[0];
    auto b1 = params1[1], b2 = params2[1];
    auto x = cvRound((b2 - b1)/(m1 - m2));
    auto y = cvRound(m1 * x + b1);
    return cv::Point(x, y);
}
void Detector::getPoints(double rho, double theta, cv::Point *points, double delta) {
    double a = cos(theta), b = sin(theta);
    double x0 = a * rho, y0 = b * rho;
    points[0].x = cvRound(x0 + delta * (-b));
    points[0].y = cvRound(y0 + delta * a);
    points[1].x = cvRound(x0 - delta * (-b));
    points[1].y = cvRound(y0 - delta * a);
}

// Callbacks
// Canny
void Detector::onChangeBlur(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->blur = 2 * pos + 1;
    det->canny_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeT1(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->T1 = pos + 1;
    det->canny_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeT2(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->T2 = pos + 1;
    det->canny_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeSobel(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->sobel = 2 * pos + 1;
    det->canny_param_changed = true;
    det->to_update = true;
}
// Lines
void Detector::onChangeRho(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->rho = pos + 1;
    det->lines_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeTheta(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->theta = deg2rad / (pos + 1);
    det->lines_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeThreshold(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->threshold = pos + 1;
    det->lines_param_changed = true;
    det->to_update = true;
}
// Circles
void Detector::onChangeAcc(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->accumulator = pos*0.1 + 1;
    det->circle_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeMinDist(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->min_dist = pos + 1;
    det->circle_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeParam1(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->param1 = pos + 200;
    det->circle_param_changed = true;
    det->to_update = true;
}
void Detector::onChangeParam2(int pos, void* d) {
    auto det = static_cast<Detector*>(d);
    det->param2 = pos + 30;
    det->circle_param_changed = true;
    det->to_update = true;
}




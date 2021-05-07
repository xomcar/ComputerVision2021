//
// Created by Marco on 27/04/2021.
//

#ifndef LAB04_HELPER_H
#define LAB04_HELPER_H

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#define deg2rad CV_PI/180

enum Type {canny, hlines, hcircles};

class Detector {
public:
    // Main functions
    Detector(cv::Mat& image, cv::String win_name); // constructor
    void closeWindow(); //toggles of window boolean
    void startCannyWindow(); //creates canny window and sets trackbars
    void startLinesWindow(); //creates hlines window and sets trackbars
    void startCirclesWindow(); //creates hcircles window and sets trackbars
    void updateWindow(); //updates window components
    cv::Mat getFinalResult();
private:
    // Private functions
    void compute(); //computes search;
    void drawStreet(); //draws street triangle
    void drawSign(); //draws street sign

    // Main storing variables
    cv::Mat og_img_color, og_img_bw, final_img;
    cv::Mat lines_img, canny_img, temp_img, circles_img;
    std::vector<cv::Vec3f> lines;
    std::vector<cv::Vec4f> circles;

    // Parameters
    int blur = 3, T1 = 10, T2 = 690, sobel = 3;
    double rho = 1, theta = 1 * deg2rad, threshold = 129;
    double accumulator = 1.1, min_dist = 1, param1 = 236, param2 = 37;

    // Window and trackbar management
    cv::String win_name;
    cv::String //bars for Canny parameters
        blur_bar = "Blur side",
        T1_bar = "T1",
        T2_bar = "T2",
        sobel_bar = "Sobel aperture";
    cv::String //bars for Hough lines parameters
        rho_bar = "Distance resolution",
        theta_bar = "Angle resolution",
        threshold_bar = "Acceptance threshold";
    cv::String //bars for Hough circles parameters
        accumulator_bar = "dp",
        min_dist_bar = "mindist",
        param1_bar = "p1",
        param2_bar = "p2";

    // Max parameters for trackbars
    int max_blur = 3, max_T1 = 800, max_T2 = max_T1 + 200, max_sobel = 3;
    double max_rho = 3, max_theta = 3, max_threshold = 500;
    double max_acc = 10, max_min_dist = 50, max_par1 = 50, max_par2 = 20;

    // Parameters for status regulation
    Type t = canny;
    bool
        win_open,
        to_update,
        canny_param_changed,
        circle_param_changed,
        lines_param_changed,
        left_1_circle,
        left_2_lines;

    // Trackbar callbacks
    // Canny
    static void onChangeBlur(int, void*);
    static void onChangeT1(int, void*);
    static void onChangeT2(int, void*);
    static void onChangeSobel(int, void*);
    // Lines
    static void onChangeTheta(int, void*);
    static void onChangeRho(int, void*);
    static void onChangeThreshold(int, void*);
    // Circles
    static void onChangeAcc(int, void*);
    static void onChangeParam1(int, void*);
    static void onChangeParam2(int, void*);
    static void onChangeMinDist(int, void*);

    // Utility functions
    static void drawCircle(cv::Mat& img, cv::Point center, double radius, cv::Scalar color); //draw a circle on an image
    static void drawLine(cv::Mat& img, double rho, double theta, double delta, cv::Scalar color); //draw a line on an image
    static cv::Vec2f getParam(cv::Point p1, cv::Point p2); //computes m and b of line from two points
    static cv::Point findIntersect(cv::Vec3f line1, cv::Vec3f line2); //find intersection point of lines
    static void getPoints(double rho, double theta, cv::Point *points, double delta); //get points from rho and theta
};

#endif //LAB04_HELPER_H

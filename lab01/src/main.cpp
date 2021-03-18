//
// Created by Marco on 18/03/2021.
//

#include "helper.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>

#define SCALE 0.8
#define SIDE 9
#define MASK_SIZE 29400
#define BRG_THRESHOLD cv::Vec3b(70, 50, 100)
#define HSV_THRESHOLD cv::Vec3b(5, 230, 230)
#define NEW_COLOR cv::Vec3b(201,37,92)

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "program usage: %executable% %filename%\n"
                     "e.g. lab01.exe robocup.jpg" << std::endl;
        return -1;
    }
    bPixel px = {0, 0, false, false};
    cv::Mat bgr_image;
    std::string WINDOW = argv[1];
    cv::namedWindow(WINDOW, cv::WINDOW_NORMAL);
    cv::moveWindow(WINDOW, 0, 0);
    if (!loadImageFromDisk(argv[1], WINDOW, bgr_image, SCALE)) {
        std::cout << argv[1] << " loaded" << std::endl;
    } else {
        std::cout << "Could not open " << argv[1] << std::endl;
        return 1;
    }
    cv::Mat hsv_image = bgr_image.clone();
    cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);
    cv::setMouseCallback(WINDOW, leftClick, &px);
    std::cout << "Image is " << bgr_image.cols << "x" << bgr_image.rows << "px" << std::endl;
    std::cout << "Right click to sample color, average BGR using " << SIDE << "x" << SIDE << " square\n"
              << "and apply BGR threshold of " << BRG_THRESHOLD << std::endl;
    std::cout << "Left click to sample color, average HSV using " << SIDE << "x" << SIDE << " square\n"
              << "and apply HSV threshold of " << HSV_THRESHOLD << std::endl;
    while(true) {
        cv::imshow(WINDOW, bgr_image);
        cv::waitKey(300); //check of input every 1/3 of a second
        if (px.l_clicked || px.r_clicked) {
            if (!checkBoundaries(px.x, px.y, SIDE, bgr_image)) {
                std::cout << "Clicked an invalid point" << std::endl;
            } else {
                std::cout << "You clicked at (" << px.x << "," << px.y << ")" << std::endl;
                cv::Mat out_rgb_img;
                if (px.r_clicked) {
                    cv::Vec3b mean_color = getMean(SIDE, px.x, px.y, bgr_image);
                    std::cout << "Mean BGR color is: " << mean_color << "\nApplying filter..." << std::endl;
                    out_rgb_img = thresholdingBRG(mean_color, NEW_COLOR, BRG_THRESHOLD, bgr_image, MASK_SIZE);
                    if (out_rgb_img.empty()) {
                        std::cout << "Selection would cause insufficient segmentation, retry in another spot!" << std::endl;
                        px.r_clicked = false;
                        px.l_clicked = false;
                        continue;
                    }
                    std::cout << "Filter applied" << std::endl;
                } else if (px.l_clicked) {
                    cv::Vec3b mean_color = getMean(SIDE, px.x, px.y, hsv_image);
                    std::cout << "Mean HSV color is: " << mean_color << "\nApplying filter..." << std::endl;
                    cv::Mat out_hsv_img = thresholdingH(mean_color, cv::Vec3b(30, 0, 0), HSV_THRESHOLD, hsv_image);
                    std::cout << "Filter applied" << std::endl;
                    cv::cvtColor(out_hsv_img, out_rgb_img, cv::COLOR_HSV2BGR);
                }
                cv::imshow(WINDOW, out_rgb_img);
                std::cout << "Press any key to remove" << std::endl;
                cv::waitKey(0);
                std::cout << "Filter removed" << std::endl;
            }
            px.l_clicked = false;
            px.r_clicked = false;
        }
        if (cv::getWindowProperty(WINDOW, cv::WND_PROP_AUTOSIZE) == -1) {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }
    }
}

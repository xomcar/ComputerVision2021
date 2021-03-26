//
// Created by Marco on 23/03/2021.
//

#include <iostream>
#include <vector>
#include <future>
#include <opencv2/highgui.hpp>

#include "helper.hpp"

#define CORNERS_ROW 6
#define CORNERS_COL 5
#define SIDE_LENGTH 1

int main(int argc, char** argv) {

    // Information parsing
    int corn_row = CORNERS_ROW;
    int corn_col = CORNERS_COL;
    double side = SIDE_LENGTH / 100.0;
    if (argc == 1) {
        std::cout << "===============================================================" << std::endl
                  << "Using default parameters:\n"
                  << "Consider using arguments of executable if using another configuration:\n"
                  << "e.g. lab2.exe 4 5 1.4\nFor 4 corner by row and 5 corners by column "
                  << "with a side of 1.4 cm\n"
                  << "===============================================================" << std::endl;
    } else if (argc == 4) {
        corn_row = std::atoi(argv[1]);
        corn_col = std::atoi(argv[2]);
        side = std::atof(argv[3]) / 100.0;
        if (!corn_row || !corn_col || !side) {
            std::cout << "Invalid input arguments!" << std::endl;
            return 1;
        }
    }
    std::cout << "Using following parameters:" << std::endl
              << "Corners by row: " << corn_row << std::endl
              << "Corners by column: " << corn_col << std::endl
              << "Side length (m) : " << side << std::endl;
    std::vector<cv::String> og_names;
    try {
        cv::glob("test/*.png", og_names);
    } catch (std::exception e){
        std::cout << "Could not find folder test\n";
        return 1;
    }

    cv::Mat original = cv::imread(og_names[0]);
    if (original.empty()) {
        std::cout << "Invalid test image for rectification, .png must be placed in \"test/\" folder!\n";
        return 1;
    } else {
        std::cout << "Loaded " << og_names[0] << " for future rectification\n";
    }

    // Loading images
    cv::Size pattern_size(corn_row, corn_col);
    std::vector<cv::String> file_names;
    std::cout << "Scanning executable directory for .png images..." << std::endl;
    cv::glob("*.png", file_names);
    auto n_images = file_names.size();
    std::cout << "Found " << n_images << " images" << std::endl;
    if (!n_images) {
        std::cout << "Exiting...\n";
        return 1;
    }
    auto img_size = cv::imread(file_names[0], cv::IMREAD_GRAYSCALE).size();
    std::vector<std::vector<cv::Point2f>> corners(n_images);
    std::vector<std::future<int>> futures(n_images);

    // Scanning and launching async computing
    int idx = 0;
    for (const auto &file : file_names) {
        cv::Mat image = imread(file, cv::IMREAD_GRAYSCALE);
        std::cout << "Loaded image " << file << std::endl;
        futures[idx] = std::async(std::launch::async, findCorners, image, std::ref(corners[idx]), pattern_size);
        idx++;
    }

    //Waiting for corners computing
    std::cout << "Computing corners, please wait!" << std::endl;
    while (true) {
        cv::waitKey(500);
        for (auto& c : futures) {
            if (c.valid()) {
                c.get();
                idx--;
            }
        }
        if (!idx) break;
    }

    //Actual camera calibration
    std::cout << "Computing done, calibrating camera..." << std::endl;
    std::vector<std::vector<cv::Point3f>> obj_points(n_images, createChessboard(pattern_size, side));
    cv::Mat camera_matrix, dist_coefficients;
    std::vector<cv::Mat> R_mats, T_mats;
    std::vector<double> per_view_errors;
    auto mean_error = cv::calibrateCamera(obj_points, corners, img_size, camera_matrix, dist_coefficients,
                                          R_mats, T_mats, cv::noArray(), cv::noArray(), per_view_errors);
    std::cout << "Mean reprojection error: " << mean_error << std::endl;
    printCameraMatrixInfo(camera_matrix);
    printDistCoeffInfo(dist_coefficients);

    // Finding best and worst results
    auto idx_min = argmin(per_view_errors);
    auto idx_max = argmax(per_view_errors);
    std::cout << "Best image: " << file_names[idx_min] << " with error " << per_view_errors[idx_min] << "%\n";
    std::cout << "Worst image: " << file_names[idx_max] << " with error " << per_view_errors[idx_max]  << "%\n";

    // Creating undistorted image
    std::cout << "Undistorting " << og_names[0] << " with newfound parameters...\n";
    cv::Mat rectified, comparison;
    cv::undistort(original, rectified, camera_matrix, dist_coefficients);
    std::cout << "Done! Close the window to exit...\n";
    cv::hconcat(original, rectified, comparison);
    cv::namedWindow("Comparison", cv::WINDOW_NORMAL);
    cv::resizeWindow("Comparison", cv::Size(0.4 * comparison.cols,0.4 * comparison.rows));
    cv::imshow("Comparison", comparison);
    while(true) {
        cv::waitKey(1000);
        if (cv::getWindowProperty("Comparison", cv::WND_PROP_AUTOSIZE) == -1) {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }
    }
}


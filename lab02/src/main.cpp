//
// Created by Marco on 23/03/2021.
//

#include <iostream>
#include <utility>
#include <vector>
#include <opencv2/highgui.hpp>

#include "helper.hpp"

#define CORNERS_ROW 6
#define CORNERS_COL 5
#define SIDE_LENGTH 1

int main(int argc, char** argv) {
    auto corn_row = CORNERS_ROW;
    auto corn_col = CORNERS_COL;
    if (argc == 1) {
        std::cout << "===============================================================" << std::endl
                  << "Using default parameters:" << std::endl
                  << "Corners by row: " << corn_row << std::endl
                  << "Corners by column: " << corn_col << std::endl
                  << "Consider using arguments of executable if using another configuration:\n"
                  << "e.g. lab2.exe 4 5\nFor 4 corner by row and 5 corners by column." << std::endl
                  << "===============================================================" << std::endl;
    } else if (argc == 3) {
        corn_row = std::atoi(argv[1]);
        corn_col = std::atoi(argv[2]);
        if (!corn_row || !corn_col) {
            std::cout << "Invalid input!" << std::endl;
            return 1;
        }
        std::cout << "Using following parameters:" << std::endl
                  << "Corners by row: " << corn_row << std::endl
                  << "Corners by column: " << corn_col << std::endl;
    }
    cv::Size pattern_size(corn_row, corn_col);
    std::vector<cv::String> file_names;
    cv::glob("*.png", file_names);
    auto n_images = file_names.size();
    auto img_size = cv::imread(file_names[0], cv::IMREAD_GRAYSCALE).size();
    std::vector<std::vector<cv::Point2f>> corners;
    for (const auto &file : file_names) {
        cv::Mat image = cv::imread(file, cv::IMREAD_GRAYSCALE);
        std::cout << "Loaded image " << file << "\nComputing corners..." << std::endl;
        std::vector<cv::Point2f> curr_corners;
        int found = findCorners(image, std::ref(curr_corners), pattern_size);
        corners.push_back(curr_corners);
        std::cout << "Found " << found << " corners" << std::endl;
    }
    std::vector<std::vector<cv::Point3f>> obj_points(n_images, createChessboard(pattern_size, SIDE_LENGTH));
    return 0;
}


//
// Created by Marco on 07/04/2021.
//
#include <opencv2/highgui.hpp>
#include <iostream>
#include <filter.h>
#include "helper_filter.h"
#define MEDIAN 0
#define GAUSSIAN 1
#define BILATERAL 2

int main(int argc, char** args) {
    if (argc == 1) {
        std::cout << "Usage: executable image.jpg\n";
        return 1;
    }
    cv::Mat src_img = cv::imread(args[1]);
    if (src_img.empty()) {
        std::cout << "Invalid image file\n";
        return 1;
    }
    cv::String filters[] = {"Median Filter", "Gaussian Filter", "Bilateral Filter"};
    while (true) {
        std::cout << "Select which filter to apply:\n" <<
            "[0] - Median\n[1] - Gaussian\n[2] - Bilateral\n[Q] - Exit\n";
        int selection;
        while (true) {
            std::cin >> selection;
            if (!std::cin) {
                std::cout << "Exiting...\n";
                return 0;
            } else if (selection < 0 || selection > 2) {
                std::cout << "Insert a value within the listed ones!\n";
            } else {
                std::cout << "Selected [" << filters[selection] << "]\n" <<
                    "Close the window to select another filter.\n";
                break;
            }
        }

        auto win_name = filters[selection];
        int kernel_size = 3;
        int max_kernel_size = 15;
        int sigma = 1;
        int max_sigma = 10;
        int sigma_range = 10;
        int max_sigma_range = 700;
        int sigma_space = 5;
        int max_sigma_space = 700;
        cv::namedWindow(win_name);
        Filter* filter;
        struct infos info =  {false};
        info.max_sigma = max_sigma;
        switch (selection) {
            default:
            case MEDIAN:
                cv::createTrackbar("kernel_size", win_name, &kernel_size,
                                   max_kernel_size, onChangeKernel, &info);
                filter = new MedianFilter(src_img, kernel_size);
                break;
            case GAUSSIAN:
                cv::createTrackbar("kernel_size", win_name, &kernel_size,
                                   max_kernel_size, onChangeKernel, &info);
                cv::createTrackbar("sigma", win_name, &sigma,
                                   max_sigma, onChangeSigma, &info);
                filter = new GaussianFilter(src_img, kernel_size, sigma);
                break;
            case BILATERAL:
                cv::createTrackbar("sigma_range", win_name, &sigma_range,
                                   max_sigma_range, onChangeSigmaRange, &info);
                cv::createTrackbar("sigma_space", win_name, &sigma_space,
                                   max_sigma_space, onChangeSigmaSpace, &info);
                filter = new BilateralFilter(src_img, kernel_size, sigma_range, sigma_space);
                break;
        }
        
        cv::imshow(win_name, src_img);
        while (true) {
            cv::waitKey(300);
            if (info.to_update) {
                switch (selection) {
                    default:
                    case MEDIAN: {
                        auto f = dynamic_cast<MedianFilter *>(filter);
                        f->setSize(info.kernel_size);
                        f->doFilter();
                        break;
                    }
                    case GAUSSIAN: {
                        auto f = dynamic_cast<GaussianFilter *>(filter);
                        f->setSize(info.kernel_size);
                        f->setSigma(info.sigma);
                        f->doFilter();
                        break;
                    }
                    case BILATERAL: {
                        auto f = dynamic_cast<BilateralFilter *>(filter);
                        f->setSize(info.kernel_size);
                        f->setSigmaSpace(info.sigma_space);
                        f->setSigmaRange(info.sigma_range);
                        f->doFilter();
                        break;
                    }
                }
                cv::imshow(win_name, filter->getResult());
                info.to_update = false;
            }
            if (cv::getWindowProperty(win_name, cv::WND_PROP_AUTOSIZE) == -1) {
                break;
            }
        }
    }
}

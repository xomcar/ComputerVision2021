//
// Created by Marco on 08/04/2021.
//
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

void onChange(int, void*);

int main(int argc, char** args) {
    cv::String win_name = "Closing demo";
    if (argc == 1) {
        std::cout << "Usage: executable image.jpg\n";
        return 1;
    }
    cv::Mat src_img = cv::imread(args[1], cv::IMREAD_GRAYSCALE);
    if (src_img.empty()) {
        std::cout << "Invalid image file\n";
        return 1;
    }
    cv::namedWindow(win_name, cv::WINDOW_GUI_NORMAL);
    cv::Mat closed_img = src_img.clone();
    cv::Mat comp;
    cv::vconcat(src_img, src_img, comp);
    int k_size = 0;
    int sel = 0;
    int max_k_size = 5;
    int max_sel = 2;
    bool update = false;
    cv::String track1 = "SHAPE (Square, Ellipse or Cross)";
    cv::String track2 = "SIZE (2*n + 1)";
    cv::createTrackbar(track1, win_name, &sel, max_sel, onChange, &update);
    cv::createTrackbar(track2, win_name, NULL, max_k_size, onChange, &update);
    cv::imshow(win_name, comp);
    while (true) {
        cv::waitKey(500);
        if (update) {
            int morph_shape = 0;
            switch (sel) {
                default:
                case 0:
                    morph_shape = cv::MORPH_RECT;
                    break;
                case 1:
                    morph_shape = cv::MORPH_ELLIPSE;
                    break;
                case 2:
                    morph_shape = cv::MORPH_CROSS;
                    break;
            }
            k_size = 2 * cv::getTrackbarPos(track2, win_name) + 1;
            auto kernel = cv::getStructuringElement(morph_shape, cv::Size(k_size,k_size));
            cv::dilate(src_img, closed_img, kernel);
            cv::erode(closed_img, closed_img, kernel);
            cv::vconcat(src_img, closed_img, comp);
            cv::imshow(win_name, comp);
            update = false;
        }
        if (cv::getWindowProperty(win_name, cv::WND_PROP_AUTOSIZE) == -1) {
            return 0;
        }
    }
}

void onChange(int pos, void* data) {
    *(static_cast<bool*>(data)) = true;
}
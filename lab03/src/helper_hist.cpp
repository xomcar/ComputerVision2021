//
// Created by Marco on 07/04/2021.
//

#include "helper_hist.h"

// hists = vector of 3 cv::mat of size nbins=256 with the 3 histograms
// e.g.: hists[0] = cv:mat of size 256 with the red histogram
//       hists[1] = cv:mat of size 256 with the green histogram
//       hists[2] = cv:mat of size 256 with the blue histogram
cv::Mat plotHistogram(std::vector<cv::Mat>& hists, int color_space)
{
    // Min/Max computation
    double hmax[3] = {0,0,0};
    double min;
    cv::minMaxLoc(hists[0], &min, &hmax[0]);
    cv::minMaxLoc(hists[1], &min, &hmax[1]);
    cv::minMaxLoc(hists[2], &min, &hmax[2]);
    cv::Scalar colors[3];
    if (!color_space) {
        colors[0] = cv::Scalar(255, 0, 0);
        colors[1] = cv::Scalar(0, 255, 0);
        colors[2] = cv::Scalar(0, 0, 255);
    } else {
        colors[0] = cv::Scalar(255, 0, 255);
        colors[1] = cv::Scalar(255, 255, 0);
        colors[2] = cv::Scalar(0, 255, 255);
    }

    std::vector<cv::Mat> canvas(hists.size());

    // Display each histogram in a canvas
    cv::Mat ret;
    for (int i = 0, end = hists.size(); i < end; i++)
    {
        canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < hists[0].rows-1; j++)
        {
            cv::line(
                    canvas[i],
                    cv::Point(j, rows),
                    cv::Point(j, rows - (hists[i].at<float>(j) * rows/hmax[i])),
                    hists.size() == 1 ? cv::Scalar(200,200,200) : colors[i],
                    1, 8, 0
            );
        }
    }
    cv::hconcat(canvas, ret);
    return ret;
}

std::vector<cv::Mat> calcHistogram(cv::Mat& img, int color_space) {
    std::vector<cv::Mat> brg_planes;
    cv::split(img, brg_planes);
    int hist_size = 256;
    float range[] = {0, (float)hist_size};
    const float* hist_range = {range};
    cv::Mat b_h, g_h, r_h;
    if (!color_space) {
        cv::calcHist(&brg_planes[0], 1, 0,
                     cv::noArray(), b_h, 1, &hist_size,
                     &hist_range, true, false);
        cv::calcHist(&brg_planes[1], 1, 0,
                     cv::noArray(), g_h, 1, &hist_size,
                     &hist_range, true, false);
        cv::calcHist(&brg_planes[2], 1, 0,
                     cv::noArray(), r_h, 1, &hist_size,
                     &hist_range, true, false);
    } else {
        cv::calcHist(&brg_planes[0], 1, 0,
                     cv::noArray(), r_h, 1, &hist_size,
                     &hist_range, true, false);
        cv::calcHist(&brg_planes[1], 1, 0,
                     cv::noArray(), g_h, 1, &hist_size,
                     &hist_range, true, false);
        cv::calcHist(&brg_planes[2], 1, 0,
                     cv::noArray(), b_h, 1, &hist_size,
                     &hist_range, true, false);
    }
    std::vector<cv::Mat> histograms = {r_h, g_h, b_h};
    return histograms;
}

cv::Mat equalizeHistogramRGB(cv::Mat& src_img) {
    std::vector<cv::Mat> planes;
    std::vector<cv::Mat> out_planes(3);
    cv::split(src_img, planes);
    int i = 0;
    for (auto plane : planes) {
        cv::equalizeHist(planes[i], out_planes[i]);
        i++;
    }
    cv::Mat out_img;
    cv::merge(out_planes, out_img);
    return out_img;
}

cv::Mat equalizeHistogramV(cv::Mat& src_img) {
    std::vector<cv::Mat> planes;
    cv::split(src_img, planes);
    cv::equalizeHist(planes[2], planes[2]);
    cv::Mat out_img;
    cv::merge(planes, out_img);
    return out_img;
}
//
// Created by Marco on 07/04/2021.
//

#ifndef LAB03_HELPER_FILTER_H
#define LAB03_HELPER_FILTER_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void onChangeKernel(int position, void* data);
void onChangeSigma(int position, void* data);
void onChangeSigmaSpace(int position, void* data);
void onChangeSigmaRange(int position, void* data);

struct infos {
    bool to_update;
    int kernel_size;
    double sigma;
    double sigma_range;
    double sigma_space;
    double max_sigma;
};
#endif //LAB03_HELPER_FILTER_H

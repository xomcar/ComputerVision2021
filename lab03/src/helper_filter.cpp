//
// Created by Marco on 07/04/2021.
//

#include "helper_filter.h"
void onChangeKernel(int position, void* data) {
    auto info = static_cast<struct infos*>(data);
    info->kernel_size = position;
    info->to_update = true;
}
void onChangeSigma(int position, void* data) {
    auto info = static_cast<struct infos*>(data);
    info->sigma = (double) position+1;
    info->to_update = true;
}

void onChangeSigmaSpace(int position, void* data) {
    auto info = static_cast<struct infos*>(data);
    info->sigma_space = position;
    info->to_update = true;
}

void onChangeSigmaRange(int position, void* data) {
    auto info = static_cast<struct infos*>(data);
    info->sigma_range = position;
    info->to_update = true;
}

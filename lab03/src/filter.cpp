#include "filter.h"

// constructor
Filter::Filter(cv::Mat input_img, int size) {

    input_image = input_img;
    if (size % 2 == 0)
        size++;
    filter_size = size;
}

// for base class do nothing (in derived classes it performs the corresponding filter)
void Filter::doFilter() {

    // it just returns a copy of the input image
    result_image = input_image.clone();
    std::cout << "cloned\n";
}

// get output of the filter
cv::Mat Filter::getResult() {

    return result_image;
}

//set window size (it needs to be odd)
void Filter::setSize(int size) {

    if (size % 2 == 0)
        size++;
    filter_size = size;
}

//get window size
int Filter::getSize() {
    return filter_size;
}

// Write your code to implement the Gaussian, median and bilateral filters

MedianFilter::MedianFilter(cv::Mat img, int size) : Filter(img, size) {}
void MedianFilter::doFilter() {
    cv::medianBlur(this->input_image, this->result_image, this->filter_size);
    //cv::imshow("Test", result_image);
    //std::cout << "Applied median filter with size " << filter_size << std::endl;
}

GaussianFilter::GaussianFilter(cv::Mat img, int size, int sigma) : Filter(img, size) {
    setSigma(sigma);
}
void GaussianFilter::setSigma(int sigma) {
    this->sigma = sigma;
}
void GaussianFilter::doFilter() {
    cv::GaussianBlur(this->input_image, this->result_image,
                     cv::Size(this->filter_size, this->filter_size), this->sigma, this->sigma);
    //std::cout << "Applied Gaussian filter with sigma " << this->sigma << std::endl;
}

BilateralFilter::BilateralFilter(cv::Mat img, int size, int sigma_range, int sigma_space) : Filter(img, size){
    setSigmaRange(sigma_range);
    setSigmaSpace(sigma_space);
}
void BilateralFilter::doFilter() {
    cv::bilateralFilter(this->input_image, this->result_image,
                        7, this->sigma_range, this->sigma_space);
    //std::cout << "Applied Bilateral filter\n";
}

void BilateralFilter::setSigmaSpace(int sigma_space) {
    this->sigma_space = sigma_space;
}

void BilateralFilter::setSigmaRange(int sigma_range) {
    this->sigma_range = sigma_range;
}

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "helper_filter.h"

// Generic class implementing a filter with the input and output image data and the parameters
class Filter{

// Methods

public:

	// constructor 
	// input_img: image to be filtered
	// filter_size : size of the kernel/window of the filter
	Filter(cv::Mat input_img, int filter_size);

    // perform filtering (in base class do nothing, to be reimplemented in the derived filters)
	void doFilter();

	// get the output of the filter
	cv::Mat getResult();

	//set the window size (square window of dimensions size x size)
	void setSize(int size);
	
	//get the Window Size
	int getSize();

	virtual ~Filter(){};

// Data

protected:

	// input image
	cv::Mat input_image;

	// output image (filter result)
	cv::Mat result_image;

	// window size
	int filter_size;



};

class GaussianFilter : public Filter  {
public:
    GaussianFilter(cv::Mat img, int size, int sigma);
    void GaussianFilter::doFilter();
    void setSigma(int sigma);

protected:
    double sigma;
};

class MedianFilter : public Filter {
public:
    MedianFilter(cv::Mat img, int size);
    void MedianFilter::doFilter();
};

class BilateralFilter : public Filter {
public:
    BilateralFilter(cv::Mat img, int size, int sigma_range, int sigma_space);
    void BilateralFilter::doFilter();
    void setSigmaRange(int sigma_range);
    void setSigmaSpace(int sigma_space);
protected:
    double sigma_range;
    double sigma_space;
};
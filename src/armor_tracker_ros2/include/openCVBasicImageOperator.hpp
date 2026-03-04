#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

class openCVBasicImageOperator
{
public:
	cv::Mat resize_Image(const cv::Mat& image, double dx, double dy, int width = 0, int height = 0);

	cv::Size get_Image_Size(const cv::Mat& image);

	cv::Mat crop_Image(const cv::Mat& image, int begin_X, int begin_Y, int width, int height);

};
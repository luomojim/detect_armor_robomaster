#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

class openCVBasicFX
{
public:
	cv::Mat to_Gray_Image(const cv::Mat& image);

	cv::Mat to_Gauss_Blur_Image(const cv::Mat& image, int sizea = 7, int sizeb = 7, double sigmaX = 0, double sigmaY = 0);

	cv::Mat to_Canny_Image(const cv::Mat &image, double threshold1 = 25, double threshold2 = 75, bool blur_preprocess = true);

	cv::Mat to_Dilate_Image(const cv::Mat& image, int sizea = 5, int sizeb = 5);

	cv::Mat to_Erode_Image(const cv::Mat& image, int sizea = 5, int sizeb = 5);

	cv::Mat to_warp_Image(const cv::Mat& image, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
	                      float width = 0, float height = 0);

	cv::Mat to_HSV_Image(const cv::Mat &image);

	cv::Mat to_MedianBlur_Image(const cv::Mat& image,int ksize = 5);

	cv::Mat to_Invert_Image(const cv::Mat& image);

	cv::Mat to_Threshold_Image(const cv::Mat& gray_image,double thresh,double maxval = 255);

	cv::Mat to_Morph_Image(const cv::Mat& image);

	cv::Mat to_Morph_Open_Image(const cv::Mat& image);

	std::vector<cv::Mat> split_image_channels(const cv::Mat& image);
};

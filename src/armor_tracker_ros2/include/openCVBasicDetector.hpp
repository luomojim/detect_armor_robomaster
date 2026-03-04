#pragma once
#include  <iostream>
#include  <opencv2/opencv.hpp>
#include "openCVBasicFX.hpp"
#include "openCVBasicOperator.hpp"
#include "openCVBasicDraw.hpp"

struct image_hsv_info
{
	//色相
	int hue_min;
	int hue_max;
	//饱和度
	int sat_min;
	int sat_max;
	//明度
	int val_min;
	int val_max;
};

class openCVBasicDetector
{
public:
	cv::Mat to_HSV_Image(const cv::Mat &image);

	cv::Mat to_Range_Hsv_Image(const cv::Mat &image, const image_hsv_info &info);

	image_hsv_info hsv_Color_Picker(const cv::Mat &image, bool hsv_preprocess = true);

	cv::Mat getCounters(cv::Mat image, cv::Mat& processed_image, int mode, bool draw = true, bool draw_rect = true,
		double area_min = 0,
		double area_max = 2147483647);

	void facial_Detection(int camera_id, std::string path);

	double getContour_area(std::vector<cv::Point> contour);

public:
	std::vector<cv::Rect> boundrects;
	std::vector<cv::Rect> circles;
	std::vector<std::vector<cv::Point> > conpolys;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchies;
};

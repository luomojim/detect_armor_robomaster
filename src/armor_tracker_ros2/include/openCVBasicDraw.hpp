#pragma once
#include  <iostream>
#include  <opencv2/opencv.hpp>

class openCVBasicDraw
{
public:
	cv::Mat create_Blank_Image(int x, int y, int r = 255, int g = 255, int b = 255);

	void draw_Circle_Image(cv::Mat image, int x, int y, int round, int thickness = 1, int r = 0, int g = 0, int b = 0);

	void draw_Rectangle_Image(cv::Mat image, int x1, int y1, int x2, int y2, int thickness = 1, int r = 0, int g = 0,
	                          int b = 0);

	void draw_Line_Image(cv::Mat image, int x1, int y1, int x2, int y2, int thickness = 1, int r = 0, int g = 0,
	                     int b = 0);

	void draw_Text(cv::Mat image, std::string words, int x, int y, double scale = 1, int thickness = 2, int r = 0,
	               int g = 0, int b = 0
	);
};

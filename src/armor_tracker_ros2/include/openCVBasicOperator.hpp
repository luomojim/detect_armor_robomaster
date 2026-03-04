#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

class openCVBasicOperator
{
public:
	std::string path;
	cv::Mat image;
	cv::VideoCapture video;
	cv::VideoWriter output;
	double video_fps = 0;
	int frame_width = 0;
	int frame_height = 0;

	void show_image(std::string window_name, cv::Mat image);

	void import_image(std::string path);

	void import_video(std::string path);

	void show_video(std::string window_name);

	void save_image(std::string filename, const cv::Mat& image);

	void get_video_info(cv::VideoCapture cap);

	void create_mp4_video_writter(std::string name);

	void release_video();
};

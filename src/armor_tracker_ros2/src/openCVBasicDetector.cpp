#include "openCVBasicDetector.hpp"

/**
 * @说明：将图像转换为HSV颜色空间
 * @输入参数：图片image
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicDetector::to_HSV_Image(const cv::Mat &image)
{
	cv::Mat output_image;
	cv::cvtColor(image, output_image, cv::COLOR_BGR2HSV);
	// 色调、饱和度、明度
	return output_image;
}

/**
 * @说明：使用Trackbar调整HSV颜色范围
 * @输入参数：图片image，是否先转换为HSV颜色空间hsv_preprocess
 * @返回值：包含调整后的HSV颜色范围的结构体image_hsv_info
 */
image_hsv_info openCVBasicDetector::hsv_Color_Picker(const cv::Mat &image, bool hsv_preprocess)
{
	image_hsv_info info;
	cv::Mat temp_image = image;
	if (hsv_preprocess == true)
	{
		temp_image = to_HSV_Image(image);
	}
	cv::namedWindow("hsv_picker", (680, 480));
	cv::createTrackbar("min hue", "hsv_picker", &info.hue_min, 179);
	cv::createTrackbar("max hue", "hsv_picker", &info.hue_max, 179);
	cv::createTrackbar("min sat", "hsv_picker", &info.sat_min, 255);
	cv::createTrackbar("max sat", "hsv_picker", &info.sat_max, 255);
	cv::createTrackbar("min val", "hsv_picker", &info.val_min, 255);
	cv::createTrackbar("max sal", "hsv_picker", &info.val_max, 255);
	cv::namedWindow("preview window", cv::WINDOW_FREERATIO);

	while (true)
	{
		cv::Mat mask;
		cv::Scalar lower(info.hue_min, info.sat_min, info.val_min);
		cv::Scalar upper(info.hue_max, info.sat_max, info.val_max);
		cv::inRange(temp_image, lower, upper, mask);
		cv::imshow("preview window", mask);
		int key = cv::waitKey(250);
		if (key == 27)
		{
			break;
		}
	}

	cv::destroyWindow("hsv_picker");
	cv::destroyWindow("preview window");
	return info;
}

/**
 * @说明：根据HSV颜色范围提取图像中的目标
 * @输入参数：图片image，HSV颜色范围结构体info
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicDetector::to_Range_Hsv_Image(const cv::Mat &image, const image_hsv_info &info)
{
	cv::Mat output_image;
	cv::Scalar lower(info.hue_min, info.sat_min, info.val_min);
	cv::Scalar upper(info.hue_max, info.sat_max, info.val_max);
	cv::inRange(image, lower, upper, output_image);
	return output_image;
}

/**
 * @说明：检测图像中的轮廓
 * @输入参数：图片image，处理后的图片processed_image，轮廓检测模式mode，是否绘制轮廓draw，是否绘制矩形draw_rect，
 *           轮廓面积最小值area_min，轮廓面积最大值area_max
 * @检测模式对照:
 * 0 : cv::RETR_EXTERNAL: 只检测最外层轮廓
 * 1 : cv::RETR_LIST: 检测所有轮廓，不建立层级关系
 * 2 : cv::RETR_CCOMP: 检测所有轮廓，建立两级层级关系
 * 3 : cv::RETR_TREE: 检测所有轮廓，建立完整的层级关系
 * @默认参数：cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, false, false, 0.0, 1000000.0
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicDetector::getCounters(cv::Mat image, cv::Mat &processed_image, int mode, bool draw, bool draw_rect,
										 double area_min,
										 double area_max)
{
	boundrects.clear();
	conpolys.clear();
	contours.clear();
	hierarchies.clear();
	// 检测到的轮廓
	std::vector<std::vector<cv::Point>> contours_find;

	// 检测到的层级
	std::vector<cv::Vec4i> hierarchy;

	// cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(7,7));
	// cv::morphologyEx(processed_image,processed_image,cv::MORPH_OPEN,kernel);

	cv::findContours(processed_image, contours_find, hierarchy, mode, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> conpoly(contours_find.size());
	std::vector<cv::Rect> boundRect(contours_find.size());

	for (int i = 0; i < contours_find.size(); i++)
	{
		float perimeter = cv::arcLength(contours_find[i], true);				 // 计算边长
		cv::approxPolyDP(contours_find[i], conpoly[i], 0.02 * perimeter, false); // 输入，输出，精度，是否闭合

		// cv::drawContours(image, conpoly, i, cv::Scalar(255, 0, 255), 2);
		boundRect[i] = cv::boundingRect(conpoly[i]);

		boundrects.push_back(boundRect[i]);
		conpolys.push_back(conpoly[i]);
		contours.push_back(contours_find[i]);
		hierarchies.push_back(hierarchy[i]);

		if (cv::contourArea(contours_find[i]) >= area_min && cv::contourArea(contours_find[i]) <= area_max)
		{
			if (draw == true)
			{
				cv::drawContours(image, conpoly, i, cv::Scalar(0, 255, 0), 3);
			}
			if (draw_rect == true)
			{
				cv::rectangle(image, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 3);
			}
		}
	}
	return image;
}

/**
 * @说明：计算轮廓的面积
 * @输入参数：轮廓contour
 * @默认参数：无
 * @返回值：轮廓面积(double)
 */
double openCVBasicDetector::getContour_area(std::vector<cv::Point> contour)
{
	return cv::contourArea(contour);
}

/**
 * @说明：人脸检测
 * @输入参数：相机ID camera_id，配置文件路径 path
 * @默认参数：0，"haarcascades/haarcascade_frontalface_default.xml"
 * @返回值：无
 */
void openCVBasicDetector::facial_Detection(int camera_id, std::string path)
{
	openCVBasicFX openCVBasicFX;
	openCVBasicDraw openCVBasicDraw;
	openCVBasicOperator openCVBasicOperator;
	cv::CascadeClassifier face_dection;
	cv::VideoCapture cap(0);

	if (!face_dection.load(cv::samples::findFile(path)))
	{
		// std::cout << "找不到配置文件" << std::endl;
		return;
	}

	while (true)
	{
		cv::Mat image;
		cap.read(image);
		cv::Mat gray_image;
		gray_image = openCVBasicFX.to_Gray_Image(image);

		std::vector<cv::Rect> faces;
		face_dection.detectMultiScale(gray_image, faces, 1.1, 15);

		if (!faces.empty())
		{
			cv::Rect main_face;
			int maxArea = 0;
			for (const auto &f : faces)
			{
				int area = f.width * f.height;
				if (area > maxArea)
				{
					maxArea = area;
					main_face = f;
				}
			}
			int x1 = main_face.x;
			int y1 = main_face.y;
			int x2 = main_face.width + x1;
			int y2 = main_face.height + y1;
			openCVBasicDraw.draw_Rectangle_Image(image, x1, y1, x2, y2, 2, 0, 255, 0);
			std::string text = "person";
			openCVBasicDraw.draw_Text(image, text, x1, y1 - 20, 1, 2, 0, 255, 0);
		}

		openCVBasicOperator.show_image("camera", image);
		int key = cv::waitKey(1);
		if (key == 27)
		{
			break;
		}
	}
}
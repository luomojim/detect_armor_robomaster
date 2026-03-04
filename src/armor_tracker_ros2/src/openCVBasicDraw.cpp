#include "openCVBasicDraw.hpp"

/**
 * @说明：创建一个空白图像
 * @输入参数：图像大小x，y，颜色r，g，b
 * @默认颜色：255 255 255
 * @返回值：空白图片(cv::mat)
 */
cv::Mat openCVBasicDraw::create_Blank_Image(int x, int y, int r, int g, int b)
{
	cv::Mat output_image(y, x, CV_8UC3, cv::Scalar(b, g, r));
	return output_image;
}

/**
 * @说明：在图像上绘制一个圆
 * @输入参数：图片image，圆心坐标x，y，半径round，线宽thickness，颜色r，g，b
 * @默认颜色：0 0 0
 * @返回值：无
 */
void openCVBasicDraw::draw_Circle_Image(cv::Mat image, int x, int y, int round, int thickness, int r, int g, int b)
{
	cv::circle(image, cv::Point(x, y), round, cv::Scalar(b, g, r), thickness);
}
/**
 * @说明：在图像上绘制一个矩形
 * @输入参数：图片image，矩形左上角坐标x1，y1，右下角坐标x2，y2，线宽thickness，颜色r，g，b
 * @默认颜色：0 0 0
 * @返回值：无
 */
void openCVBasicDraw::draw_Rectangle_Image(cv::Mat image, int x1, int y1, int x2, int y2, int thickness, int r, int g,
										   int b)
{
	cv::Point begin(x1, y1);
	cv::Point end(x2, y2);
	cv::rectangle(image, begin, end, cv::Scalar(b, g, r), thickness);
}
/**
 * @说明：在图像上绘制一条线
 * @输入参数：图片image，线的起始点坐标x1，y1，线的结束点坐标x2，y2，线宽thickness，颜色r，g，b
 * @默认颜色：0 0 0
 * @返回值：无
 */
void openCVBasicDraw::draw_Line_Image(cv::Mat image, int x1, int y1, int x2, int y2, int thickness, int r, int g, int b)
{
	cv::Point begin(x1, y1);
	cv::Point end(x2, y2);
	cv::line(image, begin, end, cv::Scalar(b, g, r), thickness);
}
/**
 * @说明：在图像上绘制文本
 * @输入参数：图片image，文本内容words，文本起始点坐标x，y，字体缩放比例scale，线宽thickness，颜色r，g，b
 * @默认颜色：0 0 0
 * @返回值：无
 */
void openCVBasicDraw::draw_Text(cv::Mat image, std::string words, int x, int y, double scale, int thickness, int r, int g, int b)
{
	cv::putText(image, words, cv::Point(x, y), cv::FONT_HERSHEY_DUPLEX, scale, cv::Scalar(b, g, r), thickness);
}

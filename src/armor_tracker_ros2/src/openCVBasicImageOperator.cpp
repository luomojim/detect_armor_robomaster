#include "openCVBasicImageOperator.hpp"

/**
 * @说明：缩放图像
 * @输入参数：图片image，缩放比例dx，dy，目标宽度width，目标高度height
 * @默认参数：无，无，0，0
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicImageOperator::resize_Image(const cv::Mat &image, double dx, double dy, int width, int height)
{
	cv::Size Size;
	if (width != 0 && height != 0)
	{
		Size = cv::Size(width, height);
	}
	cv::Mat output_image;
	cv::resize(image, output_image, Size, dx, dy);
	return output_image;
}

/**
 * @说明：获取图像大小
 * @输入参数：图片image
 * @返回值：图像大小(cv::Size)
 */
cv::Size openCVBasicImageOperator::get_Image_Size(const cv::Mat &image)
{
	return image.size();
}

/**
 * @说明：裁剪图像
 * @输入参数：图片image，裁剪起始点坐标begin_X，begin_Y，裁剪宽度width，裁剪高度height
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicImageOperator::crop_Image(const cv::Mat &image, int begin_X, int begin_Y, int width, int height)
{
	cv::Rect roi(begin_X, begin_Y, width, height);
	cv::Mat output_image = image(roi);
	return output_image;
}

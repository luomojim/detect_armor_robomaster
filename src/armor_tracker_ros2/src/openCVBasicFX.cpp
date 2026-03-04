#include "openCVBasicFX.hpp"
#include "openCVBasicDraw.hpp"

/**
 * @说明：转换成灰度图
 * @输入参数：图片
 * @返回值：处理后的图片(cv::mat类型)
 */
cv::Mat openCVBasicFX::to_Gray_Image(const cv::Mat &image)
{
	cv::Mat output_image;
	cv::cvtColor(image, output_image, cv::COLOR_BGR2GRAY);
	return output_image;
}

/**
 * @说明：对图像进行高斯模糊
 * @输入参数：输入图像，高斯核大小(cv::Size)，标准差sigmax,标准差sigmay
 * @默认参数，(7，7)，0，0
 * @高斯核：值越大模糊效果越强，通常选择奇数倍
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_Gauss_Blur_Image(const cv::Mat &image, int sizea, int sizeb, double sigmaX, double sigmaY)
{
	// 边界检查：确保图像不为空且核大小为正奇数
	if (image.empty() || sizea <= 0 || sizeb <= 0 || sizea % 2 == 0 || sizeb % 2 == 0)
	{
		return cv::Mat(); // 返回空矩阵表示错误
	}

	cv::Mat output_image;
	cv::Size Size(sizea, sizeb);
	cv::GaussianBlur(image, output_image, Size, sigmaX, sigmaY);
	return output_image;
}

/**
 * @说明：对输入的图像进行边缘化
 * @输入参数：图像，是否进行预处理，低阈值，高阈值
 * @blur_preprocess：高斯模糊预处理，如果为true，将会对图像进行高斯模糊处理
 * @默认参数25,75
 * @threshold2（高阈值）：梯度值高于此阈值的像素点被直接判定为强边缘
 * @threshold1（低阈值）：梯度值低于此阈值的像素点被直接舍弃
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_Canny_Image(const cv::Mat &image, double threshold1, double threshold2, bool blur_preprocess)
{
	cv::Mat temp_image = image;
	if (blur_preprocess == true)
	{
		temp_image = to_Gauss_Blur_Image(image);
	}
	cv::Mat output_image;
	cv::Canny(temp_image, output_image, threshold1, threshold2);
	return output_image;
}

/**
 * @说明：膨胀图像(让图像变粗)
 * @输入参数：图片，膨胀内核的大小(特别规定，传递的参数应该是奇数！！！)
 * @默认膨胀内核大小：5，5
 * @返回值：处理后的图片
 */
cv::Mat openCVBasicFX::to_Dilate_Image(const cv::Mat &image, int sizea, int sizeb)
{
	cv::Mat output_image;
	// 生成一个二值核，用来膨胀和侵蚀,cv::MORPH_RECT:矩形,cv::Size:核的大小
	cv::Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(sizea, sizeb));
	cv::dilate(image, output_image, dilate_kernel);
	return output_image;
}

/**
 * @说明：侵蚀图像(让图像变细)
 * @输入参数：图片，侵蚀内核的大小(传递的参数应该是奇数,偶数也可以，有奇妙效果)
 * @默认侵蚀内核大小：5，5
 * @返回值：处理后的图片
 */
cv::Mat openCVBasicFX::to_Erode_Image(const cv::Mat &image, int sizea, int sizeb)
{
	cv::Mat output_image;
	// 生成一个二值核，用来膨胀和侵蚀,cv::MORPH_RECT:矩形,cv::Size:核的大小
	cv::Mat erode_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(sizea, sizeb));
	cv::erode(image, output_image, erode_kernel);
	return output_image;
}

/**
 * @说明：透视变换
 * @输入参数：图片，四个点的坐标(x1,y1,x2,y2,x3,y3,x4,y4)，目标宽度，目标高度
 * @返回值：处理后的图片
 */
cv::Mat openCVBasicFX::to_warp_Image(const cv::Mat &image, float x1, float y1, float x2, float y2, float x3, float y3,
									 float x4, float y4,
									 float width, float height)
{
	if (height == 0 || width == 0)
	{
		width = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		height = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
	}
	// 左上 右上 左下 右下
	cv::Point2f source[4] = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
	cv::Point2f destination_point[4] = {{0.0f, 0.0f}, {width, 0.0f}, {0.0f, height}, {width, height}};
	openCVBasicDraw openCVBasicDraw;

	cv::Mat matrix = cv::getPerspectiveTransform(source, destination_point);

	cv::Mat output_image;
	cv::warpPerspective(image, output_image, matrix, cv::Size(width, height));
	// 在原图像上绘制点
	for (int i = 0; i < 4; i++)
	{
		openCVBasicDraw.draw_Circle_Image(image, source[i].x, source[i].y, 5, -1);
	}
	return output_image;
}

/**
 * @说明：将图像转换为HSV颜色空间
 * @输入参数：图片image
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_HSV_Image(const cv::Mat &image)
{
	cv::Mat output_image;
	cv::cvtColor(image, output_image, cv::COLOR_BGR2HSV);
	// 色调、饱和度、明度
	return output_image;
}

/**
 * @说明：中值滤波
 * @输入参数：图片image，核大小ksize(必须是奇数)
 * @默认核大小：5
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_MedianBlur_Image(const cv::Mat &image, int ksize)
{
	// 边界检查：确保图像不为空且核大小为正奇数
	if (image.empty() || ksize <= 0 || ksize % 2 == 0)
	{
		return cv::Mat(); // 返回空矩阵表示错误
	}

	cv::Mat output_image;
	cv::medianBlur(image, output_image, ksize);
	return output_image;
}

/**
 * @说明：图像取反，反色处理
 * @输入参数：图片image
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_Invert_Image(const cv::Mat &image)
{
	cv::Mat output_image = 255 - image;
	return output_image;
}

/**
 * @说明：图像二值化处理
 * @输入参数：图片gray_image，阈值thresh，最大值maxval
 * @默认阈值：无 ，255
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_Threshold_Image(const cv::Mat &gray_image, double thresh, double maxval)
{
	cv::Mat output_image;
	cv::threshold(gray_image, output_image, thresh, maxval, cv::THRESH_BINARY);
	return output_image;
}

/**
 * @说明：形态学处理
 * @输入参数：图片image
 * @默认核大小：9，9
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_Morph_Image(const cv::Mat &image)
{
	cv::Mat output_image = image;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
	morphologyEx(output_image, output_image, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 2); // 连接
	morphologyEx(output_image, output_image, cv::MORPH_OPEN, kernel);
	return output_image;
}

/**
 * @说明：形态学处理，开运算
 * @输入参数：图片image
 * @默认核大小：3，3
 * @返回值：处理后的图片(cv::mat)
 */
cv::Mat openCVBasicFX::to_Morph_Open_Image(const cv::Mat &image)
{
	cv::Mat output_image = image;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	morphologyEx(output_image, output_image, cv::MORPH_OPEN, kernel);
	return output_image;
}

/**
 * @说明：将图像通道分离rgb
 * @输入参数：图片image
 * @返回值：处理后的图片(cv::mat)
 */
std::vector<cv::Mat> openCVBasicFX::split_image_channels(const cv::Mat &image)
{
	std::vector<cv::Mat> channels;
	cv::split(image, channels);
	return channels;
}
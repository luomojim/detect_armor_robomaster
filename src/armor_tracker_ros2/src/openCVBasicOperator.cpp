#include "openCVBasicOperator.hpp"

/**
 * @说明：输出图像
 * @输入参数：新建窗口的名字；图片
 * @返回值：空
 */
void openCVBasicOperator::show_image(std::string window_name, cv::Mat image)
{
	cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	cv::imshow(window_name, image);
	while (true)
	{
		int key = cv::waitKey(1);
		if (key == 27)
		{
			break;
		}
	}
}

/**
 * @说明：导入视频
 * @输入参数：视频的相对路径
 * @返回值：空
 */
void openCVBasicOperator::import_video(std::string path)
{
	this->path = path;
	this->video.open(this->path, cv::CAP_FFMPEG);
	if (!this->video.isOpened())
	{
		this->video.open(this->path);
	}
	if (!this->video.isOpened())
	{
		std::cerr << "无法打开视频: " << this->path << std::endl;
		return;
	}
	get_video_info(this->video);
}

/**
 * @说明：导入图像
 * @输入参数：图像的相对路径
 * @返回值：空
 */
void openCVBasicOperator::import_image(std::string path)
{
	this->path = path;
	image = cv::imread(this->path);
}

/**
 * @说明：输出视频
 * @输入参数：窗口的名字
 * @返回值：空
 */
void openCVBasicOperator::show_video(std::string window_name)
{
	cv::Mat image;
	while (true)
	{
		video.read(image);
		if (!video.read(image))
		{
			break;
		}
		cv::imshow(window_name, image);
		int key = cv::waitKey(33);
		if (key == 27)
		{
			return;
		}
	}
}

/**
 * @说明：保存图片到根目录
 * @输入参数：文件名，要保存的图像，格式
 * @返回值：空
 */
void openCVBasicOperator::save_image(std::string filename, const cv::Mat &image)
{
	cv::imwrite(filename, image);
}

/**
 * @说明：获取视频信息
 * @输入参数：视频捕获对象cap
 * @返回值：无
 */
void openCVBasicOperator::get_video_info(cv::VideoCapture cap)
{
	frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	video_fps = cap.get(cv::CAP_PROP_FPS);
	if (video_fps <= 0)
		video_fps = 50.0;
}

/**
 * @说明：创建MP4视频写入器
 * @输入参数：视频文件名name
 * @返回值：无
 */
void openCVBasicOperator::create_mp4_video_writter(std::string name)
{
	if (frame_width <= 0 || frame_height <= 0)
	{
		std::cerr << "视频写入器初始化失败: 无效分辨率 " << frame_width << "x" << frame_height << std::endl;
		return;
	}
	this->output = cv::VideoWriter(name, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), video_fps, cv::Size(frame_width, frame_height));
}

/**
 * @说明：释放视频捕获对象和视频写入器
 * @输入参数：无
 * @返回值：无
 */
void openCVBasicOperator::release_video()
{
	video.release();
	output.release();
}

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#if defined(ARMOR_TRACKER_USE_MVSDK)
#include "CameraApi.h"
#endif

#include "armor_interfaces/msg/armor_info.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

#include "armorTracker.hpp"
#include "kalmanFilter.hpp"
#include "openCVBasicDetector.hpp"
#include "openCVBasicDraw.hpp"
#include "openCVBasicFX.hpp"
#include "openCVBasicImageOperator.hpp"
#include "openCVBasicOperator.hpp"

namespace
{
constexpr double kManualExposureTimeUs = 3000.0;

class ArmorCameraCapture
{
public:
	~ArmorCameraCapture()
	{
		release();
	}

	bool open()
	{
#if defined(ARMOR_TRACKER_USE_MVSDK)
		if (CameraSdkInit(1) != CAMERA_STATUS_SUCCESS)
		{
			return false;
		}

		std::vector<tSdkCameraDevInfo> camera_list(4);
		INT camera_count = static_cast<INT>(camera_list.size());
		if (CameraEnumerateDevice(camera_list.data(), &camera_count) != CAMERA_STATUS_SUCCESS || camera_count <= 0)
		{
			return false;
		}

		if (CameraInit(&camera_list[0], -1, -1, &camera_handle_) != CAMERA_STATUS_SUCCESS)
		{
			return false;
		}
		initialized_ = true;

		if (CameraGetCapability(camera_handle_, &capability_) != CAMERA_STATUS_SUCCESS)
		{
			release();
			return false;
		}
		configure_exposure();

		const bool is_mono_sensor = capability_.sIspCapacity.bMonoSensor;
		const UINT output_format = is_mono_sensor ? CAMERA_MEDIA_TYPE_MONO8 : CAMERA_MEDIA_TYPE_BGR8;
		if (CameraSetIspOutFormat(camera_handle_, output_format) != CAMERA_STATUS_SUCCESS)
		{
			release();
			return false;
		}

		const std::size_t max_width = static_cast<std::size_t>(capability_.sResolutionRange.iWidthMax);
		const std::size_t max_height = static_cast<std::size_t>(capability_.sResolutionRange.iHeightMax);
		const std::size_t output_channels = is_mono_sensor ? 1U : 3U;
		rgb_buffer_.resize(max_width * max_height * output_channels);
		if (rgb_buffer_.empty())
		{
			release();
			return false;
		}

		if (CameraPlay(camera_handle_) != CAMERA_STATUS_SUCCESS)
		{
			release();
			return false;
		}
		return true;
#else
		if (!capture_.open(0))
		{
			return false;
		}
		initialized_ = true;
		return true;
#endif
	}

	bool read(cv::Mat &frame)
	{
#if defined(ARMOR_TRACKER_USE_MVSDK)
		tSdkFrameHead frame_info{};
		BYTE *raw_buffer = nullptr;
		const CameraSdkStatus buffer_status = CameraGetImageBuffer(camera_handle_, &frame_info, &raw_buffer, 1000);
		if (buffer_status != CAMERA_STATUS_SUCCESS)
		{
			return false;
		}

		const CameraSdkStatus process_status = CameraImageProcess(camera_handle_, raw_buffer, rgb_buffer_.data(), &frame_info);
		CameraReleaseImageBuffer(camera_handle_, raw_buffer);
		if (process_status != CAMERA_STATUS_SUCCESS)
		{
			return false;
		}

		const int image_type = frame_info.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3;
		frame = cv::Mat(frame_info.iHeight, frame_info.iWidth, image_type, rgb_buffer_.data()).clone();
		if (frame.empty())
		{
			return false;
		}
		if (image_type == CV_8UC1)
		{
			cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
		}
		return true;
#else
		capture_.read(frame);
		return !frame.empty();
#endif
	}

	void release()
	{
#if defined(ARMOR_TRACKER_USE_MVSDK)
		if (initialized_)
		{
			CameraUnInit(camera_handle_);
			initialized_ = false;
		}
		rgb_buffer_.clear();
#else
		capture_.release();
		initialized_ = false;
#endif
	}

private:
#if defined(ARMOR_TRACKER_USE_MVSDK)
	void configure_exposure()
	{
		CameraSetAeState(camera_handle_, FALSE);

		double exposure_min = 0.0;
		double exposure_max = 0.0;
		double exposure_step = 0.0;
		double exposure_time = kManualExposureTimeUs;
		if (CameraGetExposureTimeRange(camera_handle_, &exposure_min, &exposure_max, &exposure_step) == CAMERA_STATUS_SUCCESS)
		{
			exposure_time = std::clamp(exposure_time, exposure_min, exposure_max);
		}
		CameraSetExposureTime(camera_handle_, exposure_time);
		CameraSetAnalogGain(camera_handle_, static_cast<INT>(capability_.sExposeDesc.uiAnalogGainMin));
	}

	CameraHandle camera_handle_{};
	tSdkCameraCapbility capability_{};
	std::vector<unsigned char> rgb_buffer_;
#else
	cv::VideoCapture capture_;
#endif
	bool initialized_ = false;
};
} // namespace

class ArmorTrackerNode : public rclcpp::Node
{
private:
	openCVBasicOperator openCVBasicOperator_;
	openCVBasicFX openCVBasicFX_;
	openCVBasicDetector openCVDetector;
	openCVBasicDraw openCVBasicDraw_;
	openCVBasicImageOperator openCVBasicImageOperator_;
	kalmanFilter kalmanFilter_;
	armorTracker armorTracker_;

	rclcpp::Publisher<armor_interfaces::msg::ArmorInfo>::SharedPtr armor_info_publisher;
	rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr center_publisher;
	std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;

	cv::Matx33d cv_to_ros;
	double delta_t = 0.02;
	cv::Mat ans;
	ArmorCameraCapture camera_capture_;
	bool camera_ready_ = false;
	bool has_last_frame_time_ = false;
	std::chrono::steady_clock::time_point last_frame_time_{};

public:
	ArmorTrackerNode()
		: Node("armor_tracker_node"),
		  cv_to_ros(0.0, 0.0, 1.0,
					-1.0, 0.0, 0.0,
					0.0, -1.0, 0.0)
	{
		armor_info_publisher = this->create_publisher<armor_interfaces::msg::ArmorInfo>("armor_info", 10);
		center_publisher = this->create_publisher<geometry_msgs::msg::PointStamped>("center", 10);
		tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

		// openCVBasicOperator_.import_video("/home/luomo/Documents/GitHub/detect_armor_robomaster/src/armor_tracker_ros2/asset/armor.mp4");
		// openCVBasicOperator_.create_mp4_video_writter("output/armor_output.mp4");
		// delta_t = 1.0 / openCVBasicOperator_.video_fps;
		// RCLCPP_INFO(this->get_logger(), "视频帧率信息(fps) %.2f, 定义的时间(s) : %.2f", openCVBasicOperator_.video_fps, delta_t);

		camera_ready_ = camera_capture_.open();
		if (!camera_ready_)
		{
			RCLCPP_ERROR(this->get_logger(), "相机初始化失败");
			return;
		}

		ans = openCVBasicDraw_.create_Blank_Image(1200, 600, 0, 0, 0);
		kalmanFilter_.init(10, 1, 1.0);
		RCLCPP_INFO(this->get_logger(), "相机初始化成功");
	}

	bool is_ready() const
	{
		return camera_ready_;
	}

	void run()
	{
		if (!camera_ready_)
		{
			return;
		}

		while (rclcpp::ok())
		{
			cv::Mat frame;
			if (!camera_capture_.read(frame))
			{
				cv::waitKey(1);
				continue;
			}

			update_delta_t();

			cv::RotatedRect rRect;
			armorTracker_.reset_armor_tracker();

			std::vector<cv::Mat> channels = openCVBasicFX_.split_image_channels(frame);
			cv::Mat detect_image = channels[0];
			detect_image = openCVBasicFX_.to_Threshold_Image(detect_image, 200, 255);

			openCVDetector.getCounters(frame, detect_image, 0, false, false, 100);
			armorTracker_.detect_armors(openCVDetector.contours);
			armorTracker_.match_armor();
			// 找到最佳的装甲板，开始绘制，解算pnp，求到核心坐标,paw
			if (armorTracker_.found == true)
			{
				// 解算pnp，yaw偏航角，pitch俯仰角，distance距离相机的距离
				armorTracker_.solve_pnp();
				armorTracker_.find_robot_center();

				double observe_speed = armorTracker_.caculate_delta_angle(delta_t);
				if (armorTracker_.is_tracking == false)
				{
					kalmanFilter_.init(10, 1, 1.0);
					armorTracker_.kf_yaw = armorTracker_.yaw;
				}
				else
				{
					kalmanFilter_.predict(delta_t);
					kalmanFilter_.update(observe_speed);

					double predicted_delta = kalmanFilter_.predict_angle_delta(delta_t);
					armorTracker_.kf_yaw = armorTracker_.yaw + predicted_delta;
					armorTracker_.kf_yaws.push_back(armorTracker_.kf_yaw);
				}

				armorTracker_.draw_result(frame, ans);

				// 广播消息
				publish_armor_info();
			}
			else
			{
				armorTracker_.is_tracking = false;
				armorTracker_.center_fits.clear();
				armorTracker_.first_frame = true;
			}

			cv::imshow("frame", frame);
			int key = cv::waitKey(1);
			if (key == 27)
			{
				break;
			}
		}

		openCVBasicOperator_.save_image("output/result.jpg", ans);
		camera_capture_.release();
		openCVBasicOperator_.release_video();
		cv::destroyAllWindows();
	}

private:
	void update_delta_t()
	{
		const auto current_frame_time = std::chrono::steady_clock::now();
		if (has_last_frame_time_)
		{
			const double measured_delta_t = std::chrono::duration<double>(current_frame_time - last_frame_time_).count();
			if (measured_delta_t > 0.0 && measured_delta_t < 1.0)
			{
				delta_t = measured_delta_t;
			}
		}
		last_frame_time_ = current_frame_time;
		has_last_frame_time_ = true;
	}

	void publish_armor_info()
	{
		// 发布消息
		armor_interfaces::msg::ArmorInfo armor_info_msg;
		armor_info_msg.yaw = armorTracker_.yaw;
		armor_info_msg.kf_yaw = armorTracker_.kf_yaw;
		armor_info_msg.pitch = armorTracker_.pitch;
		armor_info_msg.distance = armorTracker_.distance;
		armor_info_publisher->publish(armor_info_msg);

		// 发布TF
		cv::Mat rotation_mat;										// 旋转矩阵
		cv::Rodrigues(armorTracker_.rotation_vector, rotation_mat); // 使用罗德里格旋转公式变成旋转矩阵用于计算

		// 把旋转向量的黑盒显示转换出来
		cv::Matx33d rotation_cv(
			rotation_mat.at<double>(0, 0), rotation_mat.at<double>(0, 1), rotation_mat.at<double>(0, 2),
			rotation_mat.at<double>(1, 0), rotation_mat.at<double>(1, 1), rotation_mat.at<double>(1, 2),
			rotation_mat.at<double>(2, 0), rotation_mat.at<double>(2, 1), rotation_mat.at<double>(2, 2));

		// 获取三维坐标点
		cv::Vec3d translation_cv_mm(
			armorTracker_.translation_vector.at<double>(0),
			armorTracker_.translation_vector.at<double>(1),
			armorTracker_.translation_vector.at<double>(2));

		// 法向量的z轴
		cv::Vec3d normal_cv(
			rotation_cv(0, 2),
			rotation_cv(1, 2),
			rotation_cv(2, 2));

		// 计算点积，如果小于0,把y轴和z轴颠倒
		// 假如算出来的姿态目标背对相机，就把姿态翻到面向相机的那一侧，保证TF朝向一致
		if (normal_cv.dot(translation_cv_mm) < 0.0f)
		{
			cv::Matx33d trans_yz(
				1.0, 0.0, 0.0,
				0.0, -1.0, 0.0,
				0.0, 0.0, -1.0);
			// 绕x轴旋转180度，反转y和z
			rotation_cv = rotation_cv * trans_yz;
			// 重新提取第三列作为法向量
			normal_cv = cv::Vec3d(
				rotation_cv(0, 2),
				rotation_cv(1, 2),
				rotation_cv(2, 2));
		}

		// 发布装甲板的坐标系
		cv::Matx33d rotation_ros = cv_to_ros * rotation_cv;
		tf2::Matrix3x3 tf_rotation(
			rotation_ros(0, 0), rotation_ros(0, 1), rotation_ros(0, 2),
			rotation_ros(1, 0), rotation_ros(1, 1), rotation_ros(1, 2),
			rotation_ros(2, 0), rotation_ros(2, 1), rotation_ros(2, 2));
		// 创建四元数
		tf2::Quaternion q;
		tf_rotation.getRotation(q);

		geometry_msgs::msg::TransformStamped armor_tf_msg;
		armor_tf_msg.header.stamp = now();
		armor_tf_msg.header.frame_id = "camera_frame";
		armor_tf_msg.child_frame_id = "armor_frame";
		cv::Vec3d translation_cv(
			translation_cv_mm[0] * 0.001,
			translation_cv_mm[1] * 0.001,
			translation_cv_mm[2] * 0.001);
		cv::Vec3d translation_ros = cv_to_ros * translation_cv;
		armor_tf_msg.transform.translation.x = translation_ros[0];
		armor_tf_msg.transform.translation.y = translation_ros[1];
		armor_tf_msg.transform.translation.z = translation_ros[2];
		armor_tf_msg.transform.rotation.x = q.x();
		armor_tf_msg.transform.rotation.y = q.y();
		armor_tf_msg.transform.rotation.z = q.z();
		armor_tf_msg.transform.rotation.w = q.w();
		tf_broadcaster->sendTransform(armor_tf_msg);

		if (armorTracker_.find_center == true)
		{
			// 发布旋转中心
			geometry_msgs::msg::PointStamped center_point;
			center_point.header.frame_id = "camera_frame";
			center_point.header.stamp = now();

			// 转换到ros坐标系
			cv::Vec3d center_cv(
				armorTracker_.center_3d[0].x * 0.001,
				armorTracker_.center_3d[0].y * 0.001,
				armorTracker_.center_3d[0].z * 0.001);
			cv::Vec3d center_ros = cv_to_ros * center_cv;
			center_point.point.x = center_ros[0];
			center_point.point.y = center_ros[1];
			center_point.point.z = center_ros[2];
			center_publisher->publish(center_point);
		}
	}
};

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<ArmorTrackerNode>();
	if (!node->is_ready())
	{
		rclcpp::shutdown();
		return 0;
	}
	node->run();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}

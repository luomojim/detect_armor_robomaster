#include <iostream>
#include <opencv2/opencv.hpp>

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

		openCVBasicOperator_.import_video("/home/luomo/Documents/GitHub/detect_armor_robomaster/src/armor_tracker_ros2/asset/armor.mp4");
		openCVBasicOperator_.create_mp4_video_writter("output/armor_output.mp4");
		delta_t = 1.0 / openCVBasicOperator_.video_fps;
		RCLCPP_INFO(this->get_logger(), "视频帧率信息(fps) %.2f, 定义的时间(s) : %.2f", openCVBasicOperator_.video_fps, delta_t);

		ans = openCVBasicDraw_.create_Blank_Image(1200, 600, 0, 0, 0);
		kalmanFilter_.init(10, 1, 1.0);
		RCLCPP_INFO(this->get_logger(), "初始化成功");
	}

	void run()
	{
		cv::Mat frame;
		while (rclcpp::ok() && openCVBasicOperator_.video.read(frame))
		{
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

				cv::imshow("frame", frame);
				cv::waitKey(1);
			}
			else
			{
				armorTracker_.is_tracking = false;
				armorTracker_.center_fits.clear();
				armorTracker_.first_frame = true;
			}
		}

		openCVBasicOperator_.save_image("output/result.jpg", ans);
		openCVBasicOperator_.release_video();
		cv::destroyAllWindows();
	}

private:
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
	node->run();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}

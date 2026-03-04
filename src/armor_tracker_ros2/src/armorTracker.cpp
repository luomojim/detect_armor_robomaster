#include "armorTracker.hpp"
#include <cstdlib>
#include <string>

/**
 * @说明：构造函数，初始化相机内参，畸变参数，装甲板坐标系中心
 * @输入参数：无
 * @返回值：无
 */
armorTracker::armorTracker()
{
	// 针孔相机模型
	// fx 0  cx
	// 0  fy cy
	// 0  0  1
	camera_matrix = (cv::Mat_<double>(3, 3) << 2374.54248, 0.0f, 698.85288,
					 0.0f, 2377.53648, 520.8649,
					 0.0f, 0.0f, 1.0f);

	// 畸变参数
	camera_fix = (cv::Mat_<double>(1, 5) << -0.059743, 0.355479, -0.000625, 0.001595, 0.000000);

	// 坐标系：Z=0平面，原点在中心，X轴向右，Y轴向下
	object_points.clear();
	object_points.push_back(cv::Point3f(-67.5, -27.5, 0)); // 左上 (TL)
	object_points.push_back(cv::Point3f(67.5, -27.5, 0));  // 右上 (TR)
	object_points.push_back(cv::Point3f(67.5, 27.5, 0));   // 右下 (BR)
	object_points.push_back(cv::Point3f(-67.5, 27.5, 0));  // 左下 (BL)
}

/**
 * @说明：筛选灯条，符合条件的放进去
 * @输入参数：灯条轮廓contours
 * @返回值：无
 */
void armorTracker::detect_armors(std::vector<std::vector<cv::Point>> contours)
{
	light_bars.clear();

	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() < 4)
		{
			continue;
		}
		cv::RotatedRect rrect = cv::minAreaRect(contours[i]); // 拟合矩形,最少需要4个点,上面的筛子要筛掉

		// 统一角度
		if (rrect.size.width > rrect.size.height)
		{
			rrect.angle = rrect.angle - 90;
			std::swap(rrect.size.width, rrect.size.height);
		}

		// 高宽比小于2,角度差大于25度，高度小于10的过滤
		if ((rrect.size.height / rrect.size.width) < 2)
		{
			continue;
		}
		if (abs(rrect.angle) > 25.0f)
		{
			continue;
		}
		if (rrect.size.height < 10)
		{
			continue;
		}

		// 存入灯条里面
		cv::RotatedRect temp = {rrect.center, rrect.size, rrect.angle};
		light_bars.push_back(temp);
	}
}

/**
 * @说明：匹配装甲板，找到符合条件的一对灯条
 * @输入参数：无
 * @返回值：无
 */
void armorTracker::match_armor()
{
	double min_score = INT_MAX;
	found = false;

	// 进行分类，按照中心坐标从左往右排序,用lambda更简洁: [](两个比较的变量){返回比较结果}
	// 方便选出四个角点
	sort(light_bars.begin(), light_bars.end(),
		 [](const cv::RotatedRect &a, const cv::RotatedRect &b)
		 {
			 return a.center.x < b.center.x;
		 });

	// 冒泡排
	for (int i = 0; i < light_bars.size(); i++)
	{
		for (int j = i + 1; j < light_bars.size(); j++)
		{
			float angle_diff = abs(light_bars[i].angle - light_bars[j].angle);																				  // 角度差
			float height_diff = abs(light_bars[i].size.height - light_bars[j].size.height) / ((light_bars[i].size.height + light_bars[j].size.height) / 2.0); // 高度差比例
			float y_diff = abs(light_bars[i].center.y - light_bars[j].center.y) / ((light_bars[i].size.height + light_bars[j].size.height) / 2.0);			  // 中心y值的差比例
			float distance = cv::norm(light_bars[i].center - light_bars[j].center);																			  // 使用范数计算两个灯条距离
			float ratio = distance / ((light_bars[i].size.height + light_bars[j].size.height) / 2.0);														  // 两个灯条构成矩形的比例

			// 筛子
			if (angle_diff > 15)
			{
				continue;
			}
			if (height_diff > 0.5)
			{
				continue;
			}
			if (y_diff > 0.25)
			{
				continue;
			}
			if (ratio < 1.0 || ratio > 3.0)
			{
				continue;
			}

			// 计算分数，越小分数越符合条件
			double score = angle_diff + height_diff + y_diff;
			if (score < min_score)
			{
				min_score = score;
				found = true;
				armors.left_lightbar = light_bars[i];
				armors.right_lightbar = light_bars[j];
				armors.center = (light_bars[i].center + light_bars[j].center) * 0.5; // 两个灯条构成的矩形的中心
			}
		}
	}

	if (found == true)
	{
		// 计算四个角点
		cv::Point2f left_point[4], right_point[4];
		armors.left_lightbar.points(left_point);
		armors.right_lightbar.points(right_point);
		// 之前已经排过左右，可以直接找上下，按照y值从上到下排序
		std::vector<cv::Point2f> left_points(left_point, left_point + 4); // 放进容器里面才能比较
		std::vector<cv::Point2f> right_points(right_point, right_point + 4);
		sort(left_points.begin(), left_points.end(),
			 [](const cv::Point2f &a, const cv::Point2f &b)
			 {
				 return a.y < b.y;
			 });
		sort(right_points.begin(), right_points.end(),
			 [](const cv::Point2f &a, const cv::Point2f &b)
			 {
				 return a.y < b.y;
			 });
		cv::Point2f tl = (left_points[0] + left_points[1]) / 2;
		cv::Point2f bl = (left_points[2] + left_points[3]) / 2;
		cv::Point2f tr = (right_points[0] + right_points[1]) / 2;
		cv::Point2f br = (right_points[2] + right_points[3]) / 2;

		armors.corners.clear();
		armors.corners.push_back(tl); // 左上
		armors.corners.push_back(tr); // 右上
		armors.corners.push_back(br); // 左下
		armors.corners.push_back(bl); // 右下
	}
}

/**
 * @说明：绘制装甲板结果，包括灯条和中心
 * @输入参数：原始图像frame，要绘制的图像ans
 * @返回值：无
 */
void armorTracker::draw_result(cv::Mat &frame, cv::Mat &ans)
{
	openCVBasicDraw openCVBasicDraw;
	for (int i = 0; i < 4; i++)
	{
		openCVBasicDraw.draw_Line_Image(
			frame,
			armors.corners[i].x,
			armors.corners[i].y,
			armors.corners[(i + 1) % 4].x,
			armors.corners[(i + 1) % 4].y,
			2,
			0, 255, 0);
		std::string words = "(" + std::to_string((int)armors.corners[i].x) + "," + std::to_string((int)armors.corners[i].y) + ")";
		openCVBasicDraw.draw_Text(frame, words, armors.corners[i].x, armors.corners[i].y, 0.8, 1, 0, 255, 0);
	}

	// 绘制中心
	if (find_center == true)
	{
		openCVBasicDraw.draw_Circle_Image(frame, center_2d[0].x, center_2d[0].y, 8, -1, 0, 255, 0);
		openCVBasicDraw.draw_Text(frame, "center", center_2d[0].x, center_2d[0].y, 1, 1, 0, 255, 0);
	}

	// 绘制文字
	//  显示信息
	std::vector<std::string> info;
	info.push_back("Yaw : " + std::to_string(yaw));
	info.push_back("KF Yaw : " + std::to_string(kf_yaw));
	info.push_back("Pitch    : " + std::to_string(pitch));
	info.push_back("Distance : " + std::to_string(distance) + " mm");

	// 绘制文字
	for (int order = 0; order < 4; order++)
	{
		openCVBasicDraw.draw_Text(frame, info[order], 20, (40 + order * 25), 1, 2, 0, 255, 0);
	}

	// 绘制卡尔曼滤波yaw
	if (kf_yaws.size() > 1)
	{
		for (int i = 1; i < kf_yaws.size(); i++)
		{
			int x1 = (i - 1) * 2;
			int x2 = i * 2;

			int y1 = 300 - (int)(kf_yaws[i - 1] * 3);
			int y2 = 300 - (int)(kf_yaws[i] * 3);

			openCVBasicDraw.draw_Line_Image(ans, x1, y1, x2, y2, 2, 255, 0, 0);
		}
	}
	cv::imshow("ans", ans);
}

/**
 * @说明：PnP解算，计算相机到装甲板的距离和旋转角度
 * @输入参数：无
 * @返回值：无
 */
void armorTracker::solve_pnp()
{
	if (found == false)
	{
		return;
	}

	cv::solvePnP(object_points, armors.corners, camera_matrix, camera_fix, rotation_vector, translation_vector);

	distance = cv::norm(translation_vector); // 范数计算相机距离装甲板距离

	// 坐标系定义：X右，Y下，Z前
	cv::Vec3d center_position(
		translation_vector.at<double>(0),
		translation_vector.at<double>(1),
		translation_vector.at<double>(2));

	// 装甲板平面法向量（相机坐标系下）
	cv::Mat rotation_matrix;
	// 当成黑盒用吧 这里转换成旋转矩阵
	cv::Rodrigues(rotation_vector, rotation_matrix);
	// 装甲板的法向量坐标
	cv::Vec3d normal(
		rotation_matrix.at<double>(0, 2),
		rotation_matrix.at<double>(1, 2),
		rotation_matrix.at<double>(2, 2));

	// 确保法向量朝向指向车体中心的方向
	// 计算点积
	if (normal.dot(center_position) < 0.0)
	{
		normal = -normal;
	}
	double normal_norm = cv::norm(normal);
	normal /= normal_norm;

	// 定义yaw 直接采用平面法向量偏航角，供卡尔曼滤波使用
	yaw = atan2(normal[0], normal[2]) * (180 / pai);   // x / z
	pitch = atan2(normal[1], normal[2]) * (180 / pai); // y / z

	if (abs(yaw) >= 10 && abs(yaw) <= 25)
	{
		center_fits.push_back({cv::Point3f(center_position[0], center_position[1], center_position[2]),
							   cv::Point3f(normal[0], normal[1], normal[2])});
		if (center_fits.size() > center_fit_window)
		{
			center_fits.erase(center_fits.begin());
		}
	}
}

/**
 * @说明：根据相机到装甲板的距离和旋转角度，计算装甲板中心在相机坐标系中的位置
 * @输入参数：无
 * @返回值：无
 */
void armorTracker::find_robot_center()
{
	if (found == true)
	{
		if (center_fits.size() < 5)
		{
			return;
		}

		// 0矩阵
		cv::Matx33d A = cv::Matx33d::zeros();
		cv::Vec3d b(0.0, 0.0, 0.0);
		// 单位矩阵
		cv::Matx33d I = cv::Matx33d::eye();
		for (const auto &f : center_fits)
		{
			// 装甲板位置
			cv::Vec3d p(f.position.x, f.position.y, f.position.z);
			// 装甲板法向量
			cv::Vec3d n(f.normalvector.x, f.normalvector.y, f.normalvector.z);
			cv::Matx33d nnT(
				n[0] * n[0], n[0] * n[1], n[0] * n[2],
				n[1] * n[0], n[1] * n[1], n[1] * n[2],
				n[2] * n[0], n[2] * n[1], n[2] * n[2]);

			// 保留垂直分量
			cv::Matx33d P = I - nnT;
			A += P;
			b += P * p;
		}
		cv::Vec3d center = A.inv(cv::DECOMP_SVD) * b;

		center_3d.clear();
		center_3d.push_back(cv::Point3f(
			(float)(center[0]),
			(float)(center[1]),
			(float)(center[2])));
		center_2d.clear();
		cv::Mat zero_rvec = cv::Mat::zeros(3, 1, CV_64F);
		cv::Mat zero_tvec = cv::Mat::zeros(3, 1, CV_64F);
		cv::projectPoints(center_3d, zero_rvec, zero_tvec, camera_matrix, camera_fix, center_2d);
		if (center_2d.empty())
		{
			return;
		}

		cv::Point2f now_point = center_2d[0];
		cv::Point2f final_point;
		find_center = true;

		if (first_frame == true)
		{
			final_point = now_point;
			first_frame = false;
		}
		else
		{
			final_point.x = 0.6 * now_point.x + (1.0 - 0.6) * last_center_2d.x;
			final_point.y = 0.6 * now_point.y + (1.0 - 0.6) * last_center_2d.y;
		}

		// 更新历史值
		last_center_2d = final_point;

		// 存入结果供 draw_result 使用
		center_2d.clear();
		center_2d.push_back(final_point);
		return;
	}
}

/**
 * @说明：计算装甲板角度变化
 * @输入参数：时间间隔delta_t
 * @返回值：装甲板角度变化率
 */
double armorTracker::caculate_delta_angle(double delta_t)
{
	if (is_tracking == false)
	{
		last_yaw = yaw;
		is_tracking = true;
		return 0.0;
	}

	double angle_diff = yaw - last_yaw;
	if (angle_diff > 180.0)
	{
		angle_diff -= 360.0;
	}
	if (angle_diff < -180.0)
	{
		angle_diff += 360.0;
	}

	if (abs(angle_diff) > 5.0)
	{
		is_tracking = false; // 重置跟踪状态
		last_yaw = yaw;		 // 更新 last_yaw 为当前的新角度
		return 0.0;			 // 直接返回0
	}

	double measured_speed = angle_diff / delta_t;
	last_yaw = yaw;

	return measured_speed;
}

/**
 * @说明：重置跟踪状态
 * @输入参数：无
 * @返回值：无
 */
void armorTracker::reset_armor_tracker()
{
	found = false;
	find_center = false;
	center_3d.clear();
	center_2d.clear();
}

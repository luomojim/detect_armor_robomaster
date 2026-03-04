#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "openCVBasicOperator.hpp"
#include "openCVBasicFX.hpp"
#include "openCVBasicDetector.hpp"
#include "openCVBasicDraw.hpp"
#include "openCVBasicImageOperator.hpp"
#include "kalmanFilter.hpp"

#define pai CV_PI

struct armor
{
    cv::RotatedRect left_lightbar;    // 左灯条（旋转矩形）
    cv::RotatedRect right_lightbar;   // 右灯条（旋转矩形）
    std::vector<cv::Point2f> corners; // 四个角点：用于PnP解算 (左上, 右上, 右下, 左下)
    cv::Point2f center;               // 装甲板中心点
};

struct fit_data
{
    cv::Point3f position;     // 装甲板位置 (translation_vector)
    cv::Point3f normalvector; // 装甲板法向量
};

class armorTracker
{
public:
    armorTracker();

    void detect_armors(std::vector<std::vector<cv::Point>> contours);

    void match_armor();

    void draw_result(cv::Mat &frame, cv::Mat &ans);

    void solve_pnp();

    void find_robot_center();

    double caculate_delta_angle(double delta_t);

    void reset_armor_tracker();

public:
    std::vector<cv::RotatedRect> light_bars; // 灯条
    armor armors;                            // 装甲板
    bool found = false;                      // 是否找到装甲板

    cv::Mat camera_matrix;                  // 相机针孔相机模型要传入的值，焦距fx fy,光心cx cy 3x3矩阵
    cv::Mat camera_fix;                     // 畸变修正参数
    std::vector<cv::Point3f> object_points; // 重投影中心

    double yaw = 0.0f;           // 装甲板平面法向量偏航角（用于输出和滤波）
    double kf_yaw = 0.0f;        // 装甲板偏航角的卡尔曼滤波值
    std::vector<double> kf_yaws; // 装甲板偏航角的卡尔曼滤波值历史记录
    double pitch = 0.0f;         // 装甲板俯仰角
    double distance = 0.0f;      // 装甲板距离相机光心距离
    cv::Mat translation_vector;  // 装甲板平移向量
    cv::Mat rotation_vector;     // 装甲板旋转向量

    std::vector<cv::Point3f> center_3d; // 3d中心
    std::vector<cv::Point2f> center_2d; // 2d中心坐标
    std::vector<fit_data> center_fits;  // 多帧法向线数据
    int center_fit_window = 50;         // 拟合窗口帧数
    cv::Point2f last_center_2d;         // 上一帧的2d中心坐标
    bool first_frame = true;            // 是否是第一帧
    bool find_center = false;           // 是否找到中心

    double last_yaw = 0.0;    // 上个角速度
    bool is_tracking = false; // 是否连续跟踪
};

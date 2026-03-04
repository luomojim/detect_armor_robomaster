#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>

class kalmanFilter
{
public:
    kalmanFilter();

    // 初始化卡尔曼滤波参数
    void init(double process_noise, double measure_noise, double error_post);

    // 预测步骤
    void predict(double dt);

    // 更新步骤（输入观测到的角速度）
    void update(double measured_speed);

    // 获取当前平滑后的角速度
    double get_speed();

    // 获取当前角加速度
    double get_angle_acceleration();

    // 根据当前状态预测未来 predict_time 秒后转过的角度
    double predict_angle_delta(double predict_time);

private:
    // 状态向量 x = [omega, alpha]^T (角速度，角加速度)
    double x_k[2];

    // 协方差矩阵 P (2x2)
    double P_k[4];

    // 过程噪声协方差矩阵 Q (2x2)
    double Q[4];

    // 观测噪声 R
    double R;

    // 卡尔曼增益 K (2x1)
    double K[2];
};
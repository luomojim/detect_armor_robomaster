#include "kalmanFilter.hpp"

/**
 * @说明：构造函数，初始化状态为0，P矩阵为单位矩阵
 * @输入参数：无
 * @默认颜色：0 0 0
 * @返回值：无
 */
kalmanFilter::kalmanFilter()
{
    // 初始化状态为0
    x_k[0] = 0.0;
    x_k[1] = 0.0;

    // 初始化P矩阵为单位矩阵
    P_k[0] = 1.0;
    P_k[1] = 0.0;
    P_k[2] = 0.0;
    P_k[3] = 1.0;
}

/**
 * @说明：初始化卡尔曼滤波器
 * @输入参数：过程噪声process_noise，测量噪声measure_noise，初始错误估计error_post
 * @返回值：无
 */
void kalmanFilter::init(double process_noise, double measure_noise, double error_post)
{
    // 设置对角矩阵Q
    Q[0] = process_noise;
    Q[1] = 0.0;
    Q[2] = 0.0;
    Q[3] = process_noise;

    R = measure_noise;

    // 重置P
    P_k[0] = error_post;
    P_k[1] = 0.0;
    P_k[2] = 0.0;
    P_k[3] = error_post;

    // 重置状态向量
    x_k[0] = 0.0;
    x_k[1] = 0.0;
}

/**
 * @说明：预测下一个状态
 * @输入参数：时间步长dt
 * @返回值：无
 */
void kalmanFilter::predict(double dt)
{
    // 1. 预测状态 x' = F * x
    // F = [1, dt]
    //     [0, 1 ]
    // x0' = x0 + x1 * dt (速度 = 速度 + 加速度*dt)
    // x1' = x1           (加速度假定不变模型)

    double x0_new = x_k[0] + x_k[1] * dt;
    double x1_new = x_k[1];

    x_k[0] = x0_new;
    x_k[1] = x1_new;

    // 2. 预测协方差 P' = F * P * F^T + Q
    // 这里手动展开矩阵乘法以提高效率
    // P = [p0 p1]
    //     [p2 p3]

    double p0 = P_k[0];
    double p1 = P_k[1];
    double p2 = P_k[2];
    double p3 = P_k[3];

    // F * P
    double fp0 = p0 + p2 * dt;
    double fp1 = p1 + p3 * dt;
    double fp2 = p2;
    double fp3 = p3;

    // (F * P) * F^T + Q
    // F^T = [1, 0]
    //       [dt, 1]

    P_k[0] = (fp0 * 1 + fp1 * dt) + Q[0];
    P_k[1] = (fp0 * 0 + fp1 * 1) + Q[1];
    P_k[2] = (fp2 * 1 + fp3 * dt) + Q[2];
    P_k[3] = (fp2 * 0 + fp3 * 1) + Q[3];
}

/**
 * @说明：更新状态
 * @输入参数：测量值measured_speed
 * @返回值：无
 */
void kalmanFilter::update(double measured_speed)
{
    // H = [1, 0]
    // z = measured_speed

    // 1. 计算卡尔曼增益 K = P * H^T * (H * P * H^T + R)^-1
    // H * P * H^T 结果就是一个标量：P[0]
    double S = P_k[0] + R;

    // 防止太小变成0除
    if (abs(S) < 1e-6)
    {
        S = 1e-6;
    }

    // K = P * H^T / S
    // P * H^T = [p0*1 + p1*0] = [p0]
    //           [p2*1 + p3*0]   [p2]
    K[0] = P_k[0] / S;
    K[1] = P_k[2] / S;

    // 2. 更新状态 x = x' + K * (z - H * x')
    // y = z - H * x' = measured - x[0]
    double y = measured_speed - x_k[0];

    x_k[0] = x_k[0] + K[0] * y;
    x_k[1] = x_k[1] + K[1] * y;

    // 3. 更新协方差 P = (I - K * H) * P
    // I - K * H = [1, 0] - [k0] * [1, 0] = [1-k0, 0]
    //             [0, 1]   [k1]            [-k1,  1]

    double p0 = P_k[0];
    double p1 = P_k[1];
    double p2 = P_k[2];
    double p3 = P_k[3];

    // 第一行
    P_k[0] = (1 - K[0]) * p0 + 0 * p2;
    P_k[1] = (1 - K[0]) * p1 + 0 * p3;
    // 第二行
    P_k[2] = (-K[1]) * p0 + 1 * p2;
    P_k[3] = (-K[1]) * p1 + 1 * p3;
}

/**
 * @说明：获取当前速度
 * @输入参数：无
 * @返回值：当前速度
 */
double kalmanFilter::get_speed()
{
    return x_k[0];
}

/**
 * @说明：获取当前角加速度
 * @输入参数：无
 * @返回值：当前角加速度
 */
double kalmanFilter::get_angle_acceleration()
{
    return x_k[1];
}

/**
 * @说明：预测未来一段时间的角度增量
 * @输入参数：预测时间predict_time
 * @返回值：未来一段时间的角度增量
 */
double kalmanFilter::predict_angle_delta(double predict_time)
{
    // 位移公式：delta_angle = v * t + 0.5 * a * t^2
    // x_k[0] 是当前角速度，x_k[1] 是当前角加速度
    // return x_k[0] * predict_time * 0.5;
    return x_k[0] * predict_time + 0.5 * x_k[1] * predict_time * predict_time;
}
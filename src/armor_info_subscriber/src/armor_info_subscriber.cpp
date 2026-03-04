#include "rclcpp/rclcpp.hpp"
#include "armor_interfaces/msg/armor_info.hpp"

using namespace std::chrono_literals;
using armor_interfaces::msg::ArmorInfo;
using std::placeholders::_1;

class ArmorInfoSubscriber : public rclcpp::Node
{
private:
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp::Subscription<ArmorInfo>::SharedPtr subscriber;

public:
    ArmorInfoSubscriber() : Node("armor_info_subscriber")
    {
        subscriber = this->create_subscription<ArmorInfo>(
            "armor_info",
            10,
            std::bind(&ArmorInfoSubscriber::armor_info_callback, this, _1));
    }

private:
    void armor_info_callback(const ArmorInfo &msg)
    {
        RCLCPP_INFO(this->get_logger(),
                    "小车yaw: %.2f,kf_yaw: %.2f,pitch: %.2f,distance: %.2f",
                    msg.yaw,
                    msg.kf_yaw,
                    msg.pitch,
                    msg.distance);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ArmorInfoSubscriber>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
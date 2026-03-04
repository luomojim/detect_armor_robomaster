from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import os

def generate_launch_description():
    package_share = get_package_share_directory("armor_tracker_ros2")
    urdf_path = os.path.join(package_share, "urdf", "armor.urdf")
    rviz_config_path = os.path.join(package_share, "rviz", "armor_tracker.rviz")

    # 读取urdf用于发布关节连接
    with open(urdf_path, "r", encoding="utf-8") as urdf_file:
        robot_description = urdf_file.read()

    return LaunchDescription(
        [
            Node(
                package="armor_tracker_ros2",
                executable="armor_tracker_node",
                name="armor_tracker_node",
                output="screen",
            ),
            Node(
                package="tf2_ros",
                executable="static_transform_publisher",
                name="map_to_camera_frame",
                output="screen",
                arguments=["0", "0", "1", "0", "0", "0", "map", "camera_frame"],
            ),
            Node(
                package="robot_state_publisher",
                executable="robot_state_publisher",
                name="robot_state_publisher",
                output="screen",
                parameters=[{"robot_description": robot_description}],
            ),
            Node(
                package="armor_info_subscriber",
                executable="armor_info_subscriber",
                name="armor_info_subscriber",
                output="screen",
            ),
            Node(
                package="rviz2",
                executable="rviz2",
                name="rviz2",
                output="screen",
                arguments=["-d", rviz_config_path],
            ),
        ]
    )

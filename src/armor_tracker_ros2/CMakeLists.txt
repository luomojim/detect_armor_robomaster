cmake_minimum_required(VERSION 3.8)
project(armor_tracker_ros2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ament_cmake REQUIRED)
find_package(armor_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(OpenCV REQUIRED)

set(TRACKER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/armorTracker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kalmanFilter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openCVBasicDetector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openCVBasicDraw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openCVBasicFX.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openCVBasicImageOperator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/openCVBasicOperator.cpp
)

foreach(source_file IN LISTS TRACKER_SOURCES)
  if(NOT EXISTS "${source_file}")
    message(FATAL_ERROR "Required source file not found: ${source_file}")
  endif()
endforeach()

add_executable(armor_tracker_node
  src/main.cpp
  ${TRACKER_SOURCES}
)

target_include_directories(armor_tracker_node
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(armor_tracker_node ${OpenCV_LIBS})

target_compile_definitions(
  armor_tracker_node
  PRIVATE
    ORIGIN_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

ament_target_dependencies(
  armor_tracker_node
  armor_interfaces
  geometry_msgs
  rclcpp
  std_msgs
  tf2
  tf2_ros
)

install(TARGETS armor_tracker_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch urdf asset rviz
  DESTINATION share/${PROJECT_NAME}
)

ament_package()

// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from armor_interfaces:msg/ArmorInfo.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "armor_interfaces/msg/armor_info.hpp"


#ifndef ARMOR_INTERFACES__MSG__DETAIL__ARMOR_INFO__STRUCT_HPP_
#define ARMOR_INTERFACES__MSG__DETAIL__ARMOR_INFO__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__armor_interfaces__msg__ArmorInfo __attribute__((deprecated))
#else
# define DEPRECATED__armor_interfaces__msg__ArmorInfo __declspec(deprecated)
#endif

namespace armor_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ArmorInfo_
{
  using Type = ArmorInfo_<ContainerAllocator>;

  explicit ArmorInfo_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->yaw = 0.0;
      this->kf_yaw = 0.0;
      this->pitch = 0.0;
      this->distance = 0.0;
    }
  }

  explicit ArmorInfo_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->yaw = 0.0;
      this->kf_yaw = 0.0;
      this->pitch = 0.0;
      this->distance = 0.0;
    }
  }

  // field types and members
  using _yaw_type =
    double;
  _yaw_type yaw;
  using _kf_yaw_type =
    double;
  _kf_yaw_type kf_yaw;
  using _pitch_type =
    double;
  _pitch_type pitch;
  using _distance_type =
    double;
  _distance_type distance;

  // setters for named parameter idiom
  Type & set__yaw(
    const double & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__kf_yaw(
    const double & _arg)
  {
    this->kf_yaw = _arg;
    return *this;
  }
  Type & set__pitch(
    const double & _arg)
  {
    this->pitch = _arg;
    return *this;
  }
  Type & set__distance(
    const double & _arg)
  {
    this->distance = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    armor_interfaces::msg::ArmorInfo_<ContainerAllocator> *;
  using ConstRawPtr =
    const armor_interfaces::msg::ArmorInfo_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      armor_interfaces::msg::ArmorInfo_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      armor_interfaces::msg::ArmorInfo_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__armor_interfaces__msg__ArmorInfo
    std::shared_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__armor_interfaces__msg__ArmorInfo
    std::shared_ptr<armor_interfaces::msg::ArmorInfo_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ArmorInfo_ & other) const
  {
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->kf_yaw != other.kf_yaw) {
      return false;
    }
    if (this->pitch != other.pitch) {
      return false;
    }
    if (this->distance != other.distance) {
      return false;
    }
    return true;
  }
  bool operator!=(const ArmorInfo_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ArmorInfo_

// alias to use template instance with default allocator
using ArmorInfo =
  armor_interfaces::msg::ArmorInfo_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace armor_interfaces

#endif  // ARMOR_INTERFACES__MSG__DETAIL__ARMOR_INFO__STRUCT_HPP_

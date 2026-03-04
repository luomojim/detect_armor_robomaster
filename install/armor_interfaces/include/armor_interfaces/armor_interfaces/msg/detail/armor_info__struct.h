// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from armor_interfaces:msg/ArmorInfo.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "armor_interfaces/msg/armor_info.h"


#ifndef ARMOR_INTERFACES__MSG__DETAIL__ARMOR_INFO__STRUCT_H_
#define ARMOR_INTERFACES__MSG__DETAIL__ARMOR_INFO__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/ArmorInfo in the package armor_interfaces.
typedef struct armor_interfaces__msg__ArmorInfo
{
  double yaw;
  double kf_yaw;
  double pitch;
  double distance;
} armor_interfaces__msg__ArmorInfo;

// Struct for a sequence of armor_interfaces__msg__ArmorInfo.
typedef struct armor_interfaces__msg__ArmorInfo__Sequence
{
  armor_interfaces__msg__ArmorInfo * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} armor_interfaces__msg__ArmorInfo__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARMOR_INTERFACES__MSG__DETAIL__ARMOR_INFO__STRUCT_H_

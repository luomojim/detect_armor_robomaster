// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from armor_interfaces:msg/ArmorInfo.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "armor_interfaces/msg/detail/armor_info__functions.h"
#include "armor_interfaces/msg/detail/armor_info__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace armor_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void ArmorInfo_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) armor_interfaces::msg::ArmorInfo(_init);
}

void ArmorInfo_fini_function(void * message_memory)
{
  auto typed_message = static_cast<armor_interfaces::msg::ArmorInfo *>(message_memory);
  typed_message->~ArmorInfo();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ArmorInfo_message_member_array[4] = {
  {
    "yaw",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armor_interfaces::msg::ArmorInfo, yaw),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "kf_yaw",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armor_interfaces::msg::ArmorInfo, kf_yaw),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "pitch",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armor_interfaces::msg::ArmorInfo, pitch),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "distance",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armor_interfaces::msg::ArmorInfo, distance),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ArmorInfo_message_members = {
  "armor_interfaces::msg",  // message namespace
  "ArmorInfo",  // message name
  4,  // number of fields
  sizeof(armor_interfaces::msg::ArmorInfo),
  false,  // has_any_key_member_
  ArmorInfo_message_member_array,  // message members
  ArmorInfo_init_function,  // function to initialize message memory (memory has to be allocated)
  ArmorInfo_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ArmorInfo_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ArmorInfo_message_members,
  get_message_typesupport_handle_function,
  &armor_interfaces__msg__ArmorInfo__get_type_hash,
  &armor_interfaces__msg__ArmorInfo__get_type_description,
  &armor_interfaces__msg__ArmorInfo__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace armor_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<armor_interfaces::msg::ArmorInfo>()
{
  return &::armor_interfaces::msg::rosidl_typesupport_introspection_cpp::ArmorInfo_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, armor_interfaces, msg, ArmorInfo)() {
  return &::armor_interfaces::msg::rosidl_typesupport_introspection_cpp::ArmorInfo_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

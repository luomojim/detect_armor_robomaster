// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from armor_interfaces:msg/ArmorInfo.idl
// generated code does not contain a copyright notice

#include "armor_interfaces/msg/detail/armor_info__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_armor_interfaces
const rosidl_type_hash_t *
armor_interfaces__msg__ArmorInfo__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x15, 0xff, 0xf1, 0x42, 0x7e, 0xa4, 0xd3, 0x96,
      0xb9, 0x07, 0x07, 0x63, 0x9a, 0xab, 0xdb, 0x56,
      0x98, 0x9d, 0x0a, 0x81, 0x8f, 0x90, 0x52, 0xf7,
      0x4c, 0x43, 0xe6, 0xf6, 0xd3, 0x89, 0xa1, 0xf9,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char armor_interfaces__msg__ArmorInfo__TYPE_NAME[] = "armor_interfaces/msg/ArmorInfo";

// Define type names, field names, and default values
static char armor_interfaces__msg__ArmorInfo__FIELD_NAME__yaw[] = "yaw";
static char armor_interfaces__msg__ArmorInfo__FIELD_NAME__kf_yaw[] = "kf_yaw";
static char armor_interfaces__msg__ArmorInfo__FIELD_NAME__pitch[] = "pitch";
static char armor_interfaces__msg__ArmorInfo__FIELD_NAME__distance[] = "distance";

static rosidl_runtime_c__type_description__Field armor_interfaces__msg__ArmorInfo__FIELDS[] = {
  {
    {armor_interfaces__msg__ArmorInfo__FIELD_NAME__yaw, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {armor_interfaces__msg__ArmorInfo__FIELD_NAME__kf_yaw, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {armor_interfaces__msg__ArmorInfo__FIELD_NAME__pitch, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {armor_interfaces__msg__ArmorInfo__FIELD_NAME__distance, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
armor_interfaces__msg__ArmorInfo__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {armor_interfaces__msg__ArmorInfo__TYPE_NAME, 30, 30},
      {armor_interfaces__msg__ArmorInfo__FIELDS, 4, 4},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float64 yaw\n"
  "float64 kf_yaw\n"
  "float64 pitch\n"
  "float64 distance";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
armor_interfaces__msg__ArmorInfo__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {armor_interfaces__msg__ArmorInfo__TYPE_NAME, 30, 30},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 57, 57},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
armor_interfaces__msg__ArmorInfo__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *armor_interfaces__msg__ArmorInfo__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from armor_interfaces:msg/ArmorInfo.idl
// generated code does not contain a copyright notice
#include "armor_interfaces/msg/detail/armor_info__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
armor_interfaces__msg__ArmorInfo__init(armor_interfaces__msg__ArmorInfo * msg)
{
  if (!msg) {
    return false;
  }
  // yaw
  // kf_yaw
  // pitch
  // distance
  return true;
}

void
armor_interfaces__msg__ArmorInfo__fini(armor_interfaces__msg__ArmorInfo * msg)
{
  if (!msg) {
    return;
  }
  // yaw
  // kf_yaw
  // pitch
  // distance
}

bool
armor_interfaces__msg__ArmorInfo__are_equal(const armor_interfaces__msg__ArmorInfo * lhs, const armor_interfaces__msg__ArmorInfo * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // kf_yaw
  if (lhs->kf_yaw != rhs->kf_yaw) {
    return false;
  }
  // pitch
  if (lhs->pitch != rhs->pitch) {
    return false;
  }
  // distance
  if (lhs->distance != rhs->distance) {
    return false;
  }
  return true;
}

bool
armor_interfaces__msg__ArmorInfo__copy(
  const armor_interfaces__msg__ArmorInfo * input,
  armor_interfaces__msg__ArmorInfo * output)
{
  if (!input || !output) {
    return false;
  }
  // yaw
  output->yaw = input->yaw;
  // kf_yaw
  output->kf_yaw = input->kf_yaw;
  // pitch
  output->pitch = input->pitch;
  // distance
  output->distance = input->distance;
  return true;
}

armor_interfaces__msg__ArmorInfo *
armor_interfaces__msg__ArmorInfo__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  armor_interfaces__msg__ArmorInfo * msg = (armor_interfaces__msg__ArmorInfo *)allocator.allocate(sizeof(armor_interfaces__msg__ArmorInfo), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(armor_interfaces__msg__ArmorInfo));
  bool success = armor_interfaces__msg__ArmorInfo__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
armor_interfaces__msg__ArmorInfo__destroy(armor_interfaces__msg__ArmorInfo * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    armor_interfaces__msg__ArmorInfo__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
armor_interfaces__msg__ArmorInfo__Sequence__init(armor_interfaces__msg__ArmorInfo__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  armor_interfaces__msg__ArmorInfo * data = NULL;

  if (size) {
    data = (armor_interfaces__msg__ArmorInfo *)allocator.zero_allocate(size, sizeof(armor_interfaces__msg__ArmorInfo), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = armor_interfaces__msg__ArmorInfo__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        armor_interfaces__msg__ArmorInfo__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
armor_interfaces__msg__ArmorInfo__Sequence__fini(armor_interfaces__msg__ArmorInfo__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      armor_interfaces__msg__ArmorInfo__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

armor_interfaces__msg__ArmorInfo__Sequence *
armor_interfaces__msg__ArmorInfo__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  armor_interfaces__msg__ArmorInfo__Sequence * array = (armor_interfaces__msg__ArmorInfo__Sequence *)allocator.allocate(sizeof(armor_interfaces__msg__ArmorInfo__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = armor_interfaces__msg__ArmorInfo__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
armor_interfaces__msg__ArmorInfo__Sequence__destroy(armor_interfaces__msg__ArmorInfo__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    armor_interfaces__msg__ArmorInfo__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
armor_interfaces__msg__ArmorInfo__Sequence__are_equal(const armor_interfaces__msg__ArmorInfo__Sequence * lhs, const armor_interfaces__msg__ArmorInfo__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!armor_interfaces__msg__ArmorInfo__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
armor_interfaces__msg__ArmorInfo__Sequence__copy(
  const armor_interfaces__msg__ArmorInfo__Sequence * input,
  armor_interfaces__msg__ArmorInfo__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(armor_interfaces__msg__ArmorInfo);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    armor_interfaces__msg__ArmorInfo * data =
      (armor_interfaces__msg__ArmorInfo *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!armor_interfaces__msg__ArmorInfo__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          armor_interfaces__msg__ArmorInfo__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!armor_interfaces__msg__ArmorInfo__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}

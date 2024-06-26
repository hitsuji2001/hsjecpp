#ifndef _HSJE_VULKAN_COMMON_HPP__
#define _HSJE_VULKAN_COMMON_HPP__

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <string>

#include "core/hlog.hpp"

namespace hsje {
namespace vk {
void check_result(const VkResult &result,
                  const std::string &msg_if_err = "Something went wrong!");
}  // namespace vk
}  // namespace hsje

#endif  // _HSJE_VULKAN_COMMON_HPP__

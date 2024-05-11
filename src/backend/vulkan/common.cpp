#include "backend/vulkan/common.hpp"

namespace hsje {
namespace vk {
void check_result(const VkResult& result, const std::string& msg_if_err) {
  if (result != VK_SUCCESS) {
    hlog::log::error(msg_if_err);
    throw std::runtime_error("\t" + std::string(string_VkResult(result)));
  }
}
}  // namespace vk
}  // namespace hsje

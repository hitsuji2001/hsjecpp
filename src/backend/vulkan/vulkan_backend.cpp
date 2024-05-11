#include "backend/vulkan/vulkan_backend.hpp"

#include "window/glfw_window.hpp"

using namespace hlog;

namespace hsje {
VulkanBackend::VulkanBackend() {
  this->window = std::make_shared<GlfwWindow>();
  this->instance = std::make_shared<vk::Instance>(
      this->window->get_title(), this->window->get_title(),
      this->get_required_extensions());
  this->surface = std::make_shared<vk::Surface>(this->instance, this->window);
  this->devices = std::make_shared<vk::Devices>(this->instance);
  this->queue_family = std::make_shared<vk::QueueFamily>(
      this->devices,
      vk::QueueFamilyIndex(this->devices->get_physical(), this->surface));
}

std::vector<const char*> VulkanBackend::get_required_extensions() {
  std::vector<const char*> result =
      this->window->get_require_instance_extensions();
#ifdef _DEBUG
  result.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
  return result;
}
}  // namespace hsje

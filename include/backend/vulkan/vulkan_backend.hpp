#ifndef _HSJE_VULKAN_BACKEND_HPP__
#define _HSJE_VULKAN_BACKEND_HPP__

#include "backend/backend.hpp"
#include "common.hpp"
#include "devices.hpp"
#include "instance.hpp"
#include "queue_family.hpp"
#include "surface.hpp"

namespace hsje {
class VulkanBackend : public Backend {
 public:
  VulkanBackend();
  virtual ~VulkanBackend() = default;

 private:
  std::vector<const char*> get_required_extensions();

 private:
  std::shared_ptr<vk::Devices> devices;
  std::shared_ptr<vk::Instance> instance;
  std::shared_ptr<vk::Surface> surface;
  std::shared_ptr<vk::QueueFamily> queue_family;
};
}  // namespace hsje

#endif  // _HSJE_VULKAN_BACKEND_HPP__

#ifndef _HSJE_VULKAN_DEVICES_HPP__
#define _HSJE_VULKAN_DEVICES_HPP__

#include "common.hpp"
#include "instance.hpp"

namespace hsje {
namespace vk {
class PhysicalDevice {
 public:
  PhysicalDevice(const std::shared_ptr<Instance>& instance);
  VkPhysicalDevice get() const;

 public:
  static int rate_suitability(const VkPhysicalDevice& device);
  static bool is_suitable(const VkPhysicalDevice& device);

 private:
  std::shared_ptr<Instance> instance;
  VkPhysicalDevice device;
};  // PhysicalDevice

class LogicalDevice {
 public:
  LogicalDevice(const std::shared_ptr<PhysicalDevice>& device);
  virtual ~LogicalDevice();
  VkDevice get() const;

 private:
  VkDevice device;
};  // LogicalDevice

class Devices {
 public:
  Devices(const std::shared_ptr<Instance>& instance);
  virtual ~Devices() = default;

  std::shared_ptr<PhysicalDevice> get_physical() const;
  std::shared_ptr<LogicalDevice> get_logical() const;

 private:
  std::shared_ptr<PhysicalDevice> physical;
  std::shared_ptr<LogicalDevice> logical;
  std::shared_ptr<Instance> instance;
};  // Devices
}  // namespace vk
}  // namespace hsje

#endif  // _HSJE_VULKAN_DEVICES_HPP__

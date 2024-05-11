#include "backend/vulkan/devices.hpp"

#include <map>
#include <unordered_set>

#include "backend/vulkan/queue_family_index.hpp"

using namespace hlog;

namespace hsje {
namespace vk {
PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& inst)
    : instance(inst) {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(this->instance->get(), &device_count, nullptr);

  if (device_count == 0) {
    throw std::runtime_error("Failed to find GPUs with Vulkan support!");
  }
  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(this->instance->get(), &device_count,
                             devices.data());

  std::multimap<int, VkPhysicalDevice> candidates;
  for (const auto& device : devices) {
    int score = PhysicalDevice::rate_suitability(device);
    candidates.insert(std::make_pair(score, device));
  }

  for (auto it = candidates.end(); it != candidates.begin();) {
    it--;
    if (it->first > 0 and PhysicalDevice::is_suitable(it->second)) {
      this->device = it->second;
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(it->second, &props);
      log::info("Choosing device `{}`!", props.deviceName);
      return;
    }
  }

  throw std::runtime_error("Unable to find a suitable GPU!");
}

VkPhysicalDevice PhysicalDevice::get() const { return this->device; }

int PhysicalDevice::rate_suitability(const VkPhysicalDevice& device) {
  int score = 0;
  VkPhysicalDeviceFeatures device_feats;
  VkPhysicalDeviceProperties device_props;
  vkGetPhysicalDeviceProperties(device, &device_props);
  vkGetPhysicalDeviceFeatures(device, &device_feats);
  log::trace("Checking device `{}`...", device_props.deviceName);

  if (!device_feats.geometryShader) return 0;
  if (device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }
  score += device_props.limits.maxImageDimension2D;
  score += device_props.limits.maxImageDimension3D;

  return score;
}

bool PhysicalDevice::is_suitable(const VkPhysicalDevice& device) {
  QueueFamilyIndex indices(device);
  return indices.has_graphic();
}

LogicalDevice::LogicalDevice(const std::shared_ptr<PhysicalDevice>& device) {
  log::debug("Creating logical device...");

  QueueFamilyIndex indices(device->get());
  std::unordered_set<uint32_t> unique_queue_families;
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  float queue_piority = 1.0f;

  if (indices.has_graphic())
    unique_queue_families.insert(indices.get_graphic());
  if (indices.has_compute())
    unique_queue_families.insert(indices.get_compute());
  if (indices.has_present())
    unique_queue_families.insert(indices.get_present());
  if (indices.has_transfer())
    unique_queue_families.insert(indices.get_transfer());

  for (const auto& queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    create_info.queueFamilyIndex = queue_family;
    create_info.queueCount = 1;
    create_info.pQueuePriorities = &queue_piority;
    queue_create_infos.push_back(create_info);
  }

  VkPhysicalDeviceFeatures device_feats = {};
  VkDeviceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.queueCreateInfoCount = queue_create_infos.size();
  create_info.pEnabledFeatures = &device_feats;
  create_info.enabledExtensionCount = 0;
#ifdef _DEBUG
  create_info.enabledLayerCount = Instance::validation_layers.size();
  create_info.ppEnabledLayerNames = Instance::validation_layers.data();
#else
  create_info.enabledLayerCount = 0;
  create_info.ppEnabledLayerNames = nullptr;
#endif

  vk::check_result(
      vkCreateDevice(device->get(), &create_info, nullptr, &this->device),
      "Failed to create logical device!");
  log::info("Successfully created Logical device!");
}

LogicalDevice::~LogicalDevice() {
  log::debug("Destroying Vulkan logical device...");
  vkDestroyDevice(this->device, nullptr);
  log::info("Successfully destroyed Vulkan logical device!");
}

VkDevice LogicalDevice::get() const { return this->device; }

Devices::Devices(const std::shared_ptr<Instance>& inst) : instance(inst) {
  this->physical = std::make_shared<PhysicalDevice>(this->instance);
  this->logical = std::make_shared<LogicalDevice>(this->physical);
}

std::shared_ptr<PhysicalDevice> Devices::get_physical() const {
  return this->physical;
}

std::shared_ptr<LogicalDevice> Devices::get_logical() const {
  return this->logical;
}

}  // namespace vk
}  // namespace hsje

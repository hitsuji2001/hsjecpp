#include "backend/vulkan/devices.hpp"

#include <map>

using namespace hlog;

namespace hsje {
  namespace vk {
    PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& inst): instance(inst) {
      this->choose();
    }

    VkPhysicalDevice PhysicalDevice::get() const {
      return this->device;
    }

    void PhysicalDevice::choose() {
      uint32_t device_count = 0;
      vkEnumeratePhysicalDevices(this->instance->get_instance(), &device_count, nullptr);

      if (device_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
      }
      std::vector<VkPhysicalDevice> devices(device_count);
      vkEnumeratePhysicalDevices(this->instance->get_instance(), &device_count, devices.data());

      std::multimap<int, VkPhysicalDevice> candidates;
      for (const auto& device: devices) {
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

    int PhysicalDevice::rate_suitability(const VkPhysicalDevice& device) {
      int score = 0;
      VkPhysicalDeviceFeatures   device_feats;
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

      float queue_piority                       = 1.0f;
      VkDeviceQueueCreateInfo queue_create_info = {};
      queue_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_create_info.queueFamilyIndex        = indices.get_graphic();
      queue_create_info.queueCount              = 1;
      queue_create_info.pQueuePriorities        = &queue_piority;

      VkPhysicalDeviceFeatures device_feats = {};
      VkDeviceCreateInfo create_info        = {};
      create_info.sType                     = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      create_info.pQueueCreateInfos         = &queue_create_info;
      create_info.queueCreateInfoCount      = 1;
      create_info.pEnabledFeatures          = &device_feats;
      create_info.enabledLayerCount         = 0;

      vk::check_result(
          vkCreateDevice(
            device->get(),
            &create_info,
            nullptr,
            &this->device
          ),
          "Failed to create logical device!"
      );
      log::info("Successfully created Logical device!");
    }

    LogicalDevice::~LogicalDevice() {
      log::debug("Destroying Vulkan logical device...");
      vkDestroyDevice(this->device, nullptr);
      log::info("Successfully destroyed Vulkan logical device!");
    }

    VkDevice LogicalDevice::get() const {
      return this->device;
    }

    Devices::Devices(const std::shared_ptr<Instance>& inst): instance(inst) {
      this->physical = std::make_shared<PhysicalDevice>(this->instance);
      this->logical  = std::make_shared<LogicalDevice>(this->physical);
      // QueueFamilyIndex indices(this->physical->get());
      // this->queue_family = QueueFamily(this->logical->get(), indices);
    }
  }
}

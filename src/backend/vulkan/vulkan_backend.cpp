#include "backend/vulkan/vulkan_backend.hpp"
#include "window/glfw_window.hpp"

#include <map>

using namespace hlog;

namespace hsje {
  VulkanBackend::VulkanBackend() {
    this->window = std::make_shared<GlfwWindow>();
    this->window->init();
    this->instance = std::make_shared<vk::Instance>(
        this->window->get_title(),
        this->window->get_title(),
        this->get_required_extensions()
    );
    this->choose_physical_device();
    this->create_logical_device();
  }

  VulkanBackend::~VulkanBackend() {
    this->cleanup_logical_device();
  }
  
  void VulkanBackend::choose_physical_device() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(this->instance->get_instance(), &device_count, nullptr);

    if (device_count == 0) {
      throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(this->instance->get_instance(), &device_count, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device: devices) {
      int score = this->rate_device_suitability(device);
      candidates.insert(std::make_pair(score, device));
    }

    for (auto it = candidates.end(); it != candidates.begin();) {
      it--;
      if (it->first > 0 and this->device_suitable(it->second)) {
        this->physical_device = it->second;
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(it->second, &props);
        log::info("Choosing device `{}`!", props.deviceName);
        return;
      }
    }

    throw std::runtime_error("Unable to find a suitable GPU!");
  }

  bool VulkanBackend::device_suitable(const VkPhysicalDevice& device) {
    QueueFamilyIndex indices(device);
    return indices.has_graphic();
  }

  void VulkanBackend::create_logical_device() {
    log::debug("Creating logical device...");
    QueueFamilyIndex indices(this->physical_device);

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
          this->physical_device,
          &create_info,
          nullptr,
          &this->logical_device
        ),
        "Failed to create logical device!"
    );
    log::info("Successfully created Logical device!");
    this->queue_family = vk::QueueFamily(this->logical_device, indices);
  }

  void VulkanBackend::cleanup_logical_device() {
    log::debug("Destroying Vulkan logical device...");
    vkDestroyDevice(this->logical_device, nullptr);
    log::info("Successfully destroyed Vulkan logical device!");
  }

  int VulkanBackend::rate_device_suitability(const VkPhysicalDevice& device) {
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

  std::vector<const char*> VulkanBackend::get_required_extensions() {
    std::vector<const char*> result = this->window->get_require_instance_extensions();
#ifdef _DEBUG
    result.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return result;
  }
}

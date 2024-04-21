#include "backend/vulkan/vulkan_backend.hpp"

using namespace hlog;

namespace hsje {
  void VulkanBackend::init() {
    this->set_window(std::make_shared<GlfwWindow>(800, 600, "hsjengine"));
    this->get_window()->init();
    this->create_instance();
#ifdef _DEBUG
    this->debug_messenger.create(this->instance);
#endif
    this->choose_physical_device();
  }

  void VulkanBackend::cleanup() {
#ifdef _DEBUG
    this->debug_messenger.cleanup(this->instance);
#endif
    this->cleanup_instance();
  }
  
  void VulkanBackend::cleanup_instance() {
    log::debug("Destroying Vulkan Instance...");
    vkDestroyInstance(this->instance, nullptr);
    log::info("Successfully destroyed Vulkan Instance!");
  }

  void VulkanBackend::choose_physical_device() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(this->instance, &device_count, nullptr);

    if (device_count == 0) {
      throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(this->instance, &device_count, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device: devices) {
      int score = this->rate_device_suitability(device);
      candidates.insert(std::make_pair(score, device));
    }

    for (auto it = candidates.begin(); it != candidates.end(); it++) {
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
    std::vector<const char*> result = this->get_window()->get_require_instance_extensions();
#ifdef _DEBUG
    result.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return result;
  }

  void VulkanBackend::create_instance() {
    const auto& app_info = this->create_app_info(
        this->get_window()->get_title(),
        this->get_window()->get_title()
    );
    const auto& required_extensions = this->get_required_extensions();
    const auto& create_info = this->create_instance_info(app_info, required_extensions);

    vk::check_result(
        vkCreateInstance(&create_info, nullptr, &this->instance),
        "Failed to create Vulkan Instance"
    );
  }

  VkInstanceCreateInfo VulkanBackend::create_instance_info(
      const VkApplicationInfo& app_info,
      const std::vector<const char*>& required_extensions
  ) {
    log::debug("Creating application create infomation...");
    VkInstanceCreateInfo create_info    = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo        = &app_info;
    create_info.enabledExtensionCount   = required_extensions.size();
    create_info.ppEnabledExtensionNames = required_extensions.data();
#ifdef _DEBUG
    this->debug_messenger.setup();
    create_info.enabledLayerCount   = vk::DebugMessenger::validation_layers.size();
    create_info.ppEnabledLayerNames = vk::DebugMessenger::validation_layers.data();
    create_info.pNext               = (VkDebugUtilsMessengerCreateInfoEXT*)this->debug_messenger.get_debug_messenger();
#else
    create_info.enabledLayerCount       = 0;
    create_info.ppEnabledLayerNames     = nullptr;
    create_info.pNext                   = nullptr;
#endif

    log::info("Successfully created application create infomation!");
    return create_info;
  }

  VkApplicationInfo VulkanBackend::create_app_info(
      const std::string& app_name,
      const std::string& engine_name
  ) const {
    log::debug("Creating application infomation...");
    VkApplicationInfo app_info  = {};

    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = app_name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = engine_name.c_str();
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_0;

    log::info("Successfully created application infomation!");
    return app_info;
  }
}

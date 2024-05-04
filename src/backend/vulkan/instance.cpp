#include "backend/vulkan/instance.hpp"

using namespace hlog;

namespace hsje {
  namespace vk {
    Instance::Instance(
        const std::string& app_name,
        const std::string& instance_name,
        const std::vector<const char*>& required_extensions
    ) {
      const auto& app_info    = this->create_app_info(app_name, instance_name);
      const auto& create_info = this->create_instance_info(app_info, required_extensions);

      vk::check_result(
          vkCreateInstance(&create_info, nullptr, &this->instance),
          "Failed to create Vulkan Instance"
      );
#ifdef _DEBUG
      log::debug("Creating Debug Messenger...");
      vk::check_result(
          this->create_debug_utils_messenger_ext(
            this->instance,
            &this->debug_create_info,
            nullptr,
            &this->debug_messenger
          ),
          "Failed to setup debug messenger!"
      );
#endif
    }

    Instance::~Instance() {
#ifdef _DEBUG
      log::debug("Destroying Vulkan debug utils messenger...");
      this->destroy_debug_utils_messenger_ext(this->instance, this->debug_messenger, nullptr);
      log::info("Successfully destroyed Vulkan debug utils messenger!");
#endif
      log::debug("Destroying Vulkan Instance...");
      vkDestroyInstance(this->instance, nullptr);
      log::info("Successfully destroyed Vulkan Instance!");
    }

    VkInstance Instance::get_instance() const {
      return this->instance;
    }

    VkApplicationInfo Instance::create_app_info(
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

    VkInstanceCreateInfo Instance::create_instance_info(
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
      log::debug("Setting up Debug Messenger...");
      if (!this->check_validation_layers_support()) {
        throw std::runtime_error("Validation layers requested, but not available!");
      }
      this->debug_create_info = this->create_debug_messenger_create_info();
      create_info.enabledLayerCount   = vk::Instance::validation_layers.size();
      create_info.ppEnabledLayerNames = vk::Instance::validation_layers.data();
      create_info.pNext               = (VkDebugUtilsMessengerCreateInfoEXT*) &this->debug_create_info;
#else
      create_info.enabledLayerCount       = 0;
      create_info.ppEnabledLayerNames     = nullptr;
      create_info.pNext                   = nullptr;
#endif // _DEBUG

      log::info("Successfully created application create infomation!");
      return create_info;
    }

#ifdef _DEBUG
    const std::vector<const char*> Instance::validation_layers = {
      "VK_LAYER_KHRONOS_validation"
    };

    bool Instance::check_validation_layers_support() const {
      uint32_t layer_count;
      vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
      std::vector<VkLayerProperties> available_layers(layer_count);
      vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

      log::debug("Supported {} layer(s):", layer_count);
      for (const auto& layer: available_layers) {
        log::trace("  {}", layer.layerName);
      }

      for (const auto& name: Instance::validation_layers) {
        bool found = false;
        log::trace("Finding `{}` layer...", name);
        for (const auto& props: available_layers) {
          if (std::string(name).compare(props.layerName) == 0) {
            log::trace("Found!");
            found = true;
            break;
          }
        }
        if (!found) {
          log::trace("Could not find {}", name);
          return false;
        }
      }
      log::info("Successfully found all validation layers!");
      return true;
    }

    VkDebugUtilsMessengerCreateInfoEXT Instance::create_debug_messenger_create_info() const {
      VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
      debug_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      debug_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      debug_create_info.pfnUserCallback = Instance::debug_callback;
      return debug_create_info;
    }

    VkResult Instance::create_debug_utils_messenger_ext(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* create_info,
        const VkAllocationCallbacks* allocator,
        VkDebugUtilsMessengerEXT* debug_messenger
    ) {
      const auto& func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
          instance,
          "vkCreateDebugUtilsMessengerEXT"
          );
      if (func != nullptr) return func(instance, create_info, allocator, debug_messenger);
      else return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void Instance::destroy_debug_utils_messenger_ext(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debug_messenger,
        const VkAllocationCallbacks* allocator
    ) {
      const auto& func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
          instance,
          "vkDestroyDebugUtilsMessengerEXT"
          );
      if (func != nullptr) func(instance, debug_messenger, allocator);
    }

    std::string Instance::debug_message_type_to_string(
        const VkDebugUtilsMessageTypeFlagsEXT& type
    ) {
      switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:     return "General";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:  return "Validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "Performance";
        default: return "???";
      }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
        VkDebugUtilsMessageTypeFlagsEXT msg_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    ) {
      (void) user_data;
      if (msg_severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) return VK_FALSE;

      std::string type = Instance::debug_message_type_to_string(msg_type);
      const std::string message = fmt::format(
          "[{} - {}]: {}",
          "Validation Layer",
          type,
          callback_data->pMessage
      );
      switch (msg_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
          log::trace(message);
          break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
          log::warn(message);
          break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
          log::info(message);
          break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
          log::error(message);
          break;
        default:
          break;
      }

      return VK_FALSE;
    }
#endif // _DEBUG
  }
}

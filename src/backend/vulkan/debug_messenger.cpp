#include "backend/vulkan/debug_messenger.hpp"

using namespace hlog;

namespace hsje {
  namespace vk {
    const std::vector<const char*> DebugMessenger::validation_layers = {
      "VK_LAYER_KHRONOS_validation"
    };

    VkDebugUtilsMessengerEXT DebugMessenger::get_debug_messenger() const {
      return this->debug_messenger;
    }

    void DebugMessenger::setup() {
      log::debug("Setting up Debug Messenger...");
      if (!this->check_validation_layers_support()) {
        throw std::runtime_error("Validation layers requested, but not available!");
      }
      this->debug_create_info = this->create_debug_messenger_create_info();
    }

    void DebugMessenger::create(const VkInstance& instance) {
      log::debug("Creating Debug Messenger...");
      vk::check_result(
          this->create_debug_utils_messenger_ext(
            instance,
            &this->debug_create_info,
            nullptr,
            &this->debug_messenger
            ),
          "Failed to setup debug messenger!"
      );
    }

    void DebugMessenger::cleanup(const VkInstance& instance) {
      log::debug("Destroying Vulkan debug utils messenger...");
      this->destroy_debug_utils_messenger_ext(instance, this->debug_messenger, nullptr);
      log::info("Successfully destroyed Vulkan debug utils messenger!");
    }

    bool DebugMessenger::check_validation_layers_support() const {
      uint32_t layer_count;
      vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
      std::vector<VkLayerProperties> available_layers(layer_count);
      vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

      log::debug("Supported {} layer(s):", layer_count);
      for (const auto& name: DebugMessenger::validation_layers) {
        bool found = false;
        log::trace("Finding `{}` layer...", name);
        for (const auto& props: available_layers) {
          if (strcmp(name, props.layerName) == 0) {
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

    VkDebugUtilsMessengerCreateInfoEXT DebugMessenger::create_debug_messenger_create_info() const {
      VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
      debug_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      debug_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      debug_create_info.pfnUserCallback = this->debug_callback;
      return debug_create_info;
    }

    VkResult DebugMessenger::create_debug_utils_messenger_ext(
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

    void DebugMessenger::destroy_debug_utils_messenger_ext(
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

    std::string DebugMessenger::debug_message_type_to_string(
        const VkDebugUtilsMessageTypeFlagsEXT& type
    ) {
      switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:     return "General";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:  return "Validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "Performance";
        default: return "???";
      }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
        VkDebugUtilsMessageTypeFlagsEXT msg_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    ) {
      (void) user_data;
      if (msg_severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) return VK_FALSE;

      std::string type = DebugMessenger::debug_message_type_to_string(msg_type);
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
  }
}

#ifndef _HSJE_VULKAN_DEBUG_MESSENGER_HPP__
#define _HSJE_VULKAN_DEBUG_MESSENGER_HPP__

#ifdef _DEBUG

#include <vector>
#include <string>

#include <vulkan/vulkan_core.h>

#include "common.hpp"

namespace hsje {
  namespace vk {
    class DebugMessenger {
      public:
        void setup();
        void cleanup(const VkInstance& instance);
        void create(const VkInstance& instance);
        VkDebugUtilsMessengerEXT get_debug_messenger() const;
      public:
        static const std::vector<const char*> validation_layers;

      private:
        bool check_validation_layers_support() const;
        VkDebugUtilsMessengerCreateInfoEXT create_debug_messenger_create_info() const;
        VkResult create_debug_utils_messenger_ext(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* create_info,
            const VkAllocationCallbacks* allocator,
            VkDebugUtilsMessengerEXT* debug_messenger
        );
        void destroy_debug_utils_messenger_ext(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debug_messenger,
            const VkAllocationCallbacks* allocator
        );
      private:
        static std::string debug_message_type_to_string(
            const VkDebugUtilsMessageTypeFlagsEXT& type
        );
        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
            VkDebugUtilsMessageTypeFlagsEXT msg_type,
            const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
            void* user_data
        );
      private:
        VkDebugUtilsMessengerEXT debug_messenger;
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
    };
  }
}

#endif

#endif // _HSJE_VULKAN_DEBUG_MESSENGER_HPP__

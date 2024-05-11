#ifndef _HSJE_VULKAN_INSTANCE_HPP__
#define _HSJE_VULKAN_INSTANCE_HPP__

#include <vector>

#include "common.hpp"

namespace hsje {
namespace vk {
class Instance {
 public:
  Instance(const std::string& app_name, const std::string& instance_name,
           const std::vector<const char*>& required_extensions);
  virtual ~Instance();
  VkInstance get() const;
#ifdef _DEBUG
 public:
  static const std::vector<const char*> validation_layers;
#endif

 private:
  VkApplicationInfo create_app_info(const std::string& app_name,
                                    const std::string& engine_name) const;
  VkInstanceCreateInfo create_instance_info(
      const VkApplicationInfo& app_info,
      const std::vector<const char*>& required_extensions);

#ifdef _DEBUG
 private:
  bool check_validation_layers_support() const;
  VkDebugUtilsMessengerCreateInfoEXT create_debug_messenger_create_info() const;
  VkResult create_debug_utils_messenger_ext(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT* create_info,
      const VkAllocationCallbacks* allocator,
      VkDebugUtilsMessengerEXT* debug_messenger);
  void destroy_debug_utils_messenger_ext(
      VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
      const VkAllocationCallbacks* allocator);

 private:
  static std::string debug_message_type_to_string(
      const VkDebugUtilsMessageTypeFlagsEXT& type);
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
                 VkDebugUtilsMessageTypeFlagsEXT msg_type,
                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                 void* user_data);

#endif  // _DEBUG

 private:
  VkInstance instance;
#ifdef _DEBUG
  VkDebugUtilsMessengerEXT debug_messenger;
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
#endif  // _DEBUG
};
}  // namespace vk
}  // namespace hsje

#endif  // _HSJE_VULKAN_INSTANCE_HPP__

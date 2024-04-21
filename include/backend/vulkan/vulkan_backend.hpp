#ifndef _HSJE_VULKAN_BACKEND_HPP__
#define _HSJE_VULKAN_BACKEND_HPP__

#include <map>

#include <vulkan/vulkan_core.h>

#include "common.hpp"
#include "debug_messenger.hpp"
#include "queue_family.hpp"

#include "core/hlog.hpp"
#include "backend/backend.hpp"
#include "window/glfw_window.hpp"

namespace hsje {
  class VulkanBackend: public Backend {
    public:
      void init() override;
      void cleanup() override;

    private:
      void create_instance();
      void cleanup_instance();

      int rate_device_suitability(const VkPhysicalDevice& device);
      void choose_physical_device();
      bool device_suitable(const VkPhysicalDevice& device);

      std::vector<const char*> get_required_extensions();
      VkInstanceCreateInfo create_instance_info(
          const VkApplicationInfo& app_info,
          const std::vector<const char*>& required_extensions
      );
      VkApplicationInfo create_app_info(
          const std::string& app_name,
          const std::string& engine_name
      ) const;
    private:
      VkInstance         instance;
      VkPhysicalDevice   physical_device;
#ifdef _DEBUG
      vk::DebugMessenger debug_messenger;
#endif
  };
}

#endif // _HSJE_VULKAN_BACKEND_HPP__

#ifndef _HSJE_VULKAN_BACKEND_HPP__
#define _HSJE_VULKAN_BACKEND_HPP__

#include "common.hpp"
#include "instance.hpp"
#include "queue_family.hpp"

#include "backend/backend.hpp"

namespace hsje {
  class VulkanBackend: public Backend {
    public:
      VulkanBackend();
      virtual ~VulkanBackend();

    private:
      void choose_physical_device();
      int  rate_device_suitability(const VkPhysicalDevice& device);
      bool device_suitable(const VkPhysicalDevice& device);

      void create_logical_device();
      void cleanup_logical_device();

      std::vector<const char*> get_required_extensions();
    private:
      VkPhysicalDevice                    physical_device;
      VkDevice                            logical_device;
      vk::QueueFamily                     queue_family;
      std::shared_ptr<vk::Instance>       instance;
  };
}

#endif // _HSJE_VULKAN_BACKEND_HPP__

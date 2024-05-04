#ifndef _HSJE_VULKAN_BACKEND_HPP__
#define _HSJE_VULKAN_BACKEND_HPP__

#include "common.hpp"
#include "devices.hpp"
#include "instance.hpp"
#include "queue_family.hpp"

#include "backend/backend.hpp"

namespace hsje {
  class VulkanBackend: public Backend {
    public:
      VulkanBackend();
      virtual ~VulkanBackend();

    private:
      std::vector<const char*> get_required_extensions();

    private:
      std::shared_ptr<vk::Devices>  devices;
      std::shared_ptr<vk::Instance> instance;
  };
}

#endif // _HSJE_VULKAN_BACKEND_HPP__

#ifndef _HSJE_VULKAN_QUEUE_FAMILY_HPP__
#define _HSJE_VULKAN_QUEUE_FAMILY_HPP__

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace hsje {
  class QueueFamilyIndex {
    public:
      QueueFamilyIndex(const VkPhysicalDevice& device);
      virtual ~QueueFamilyIndex();

      bool has_graphic() const;
      bool has_compute() const;
      bool has_transfer() const;
      bool has_present() const;

      uint32_t get_graphic()  const;
      uint32_t get_compute()  const;
      uint32_t get_transfer() const;
      uint32_t get_present()  const;

    private:
      uint32_t graphic;
      uint32_t compute;
      uint32_t transfer;
      uint32_t present;

      static const uint32_t _not_present;
  };
}

#endif // _HSJE_VULKAN_QUEUE_FAMILY_HPP__

#ifndef _HSJE_VULKAN_QUEUE_FAMILY_INDEX_HPP__
#define _HSJE_VULKAN_QUEUE_FAMILY_INDEX_HPP__

#include <vector>

#include "common.hpp"

namespace hsje {
  enum class QueueFamilyType {
    Graphic,
    Compute,
    Transfer,
    Present,
    Size
  };

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
      std::vector<uint32_t> indices;
      static const uint32_t _not_present;
  };
}

#endif // _HSJE_VULKAN_QUEUE_FAMILY_INDEX_HPP__

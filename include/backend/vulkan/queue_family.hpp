#ifndef _HSJE_VULKAN_QUEUE_FAMILY_HPP__
#define _HSJE_VULKAN_QUEUE_FAMILY_HPP__

#include "common.hpp"
#include "queue_family_index.hpp"

namespace hsje {
  namespace vk {
    class QueueFamily {
      public:
        QueueFamily();
        QueueFamily(const VkDevice& logical_device, const QueueFamilyIndex& indices);
        virtual ~QueueFamily();

        VkQueue get_graphic()  const;
        VkQueue get_compute()  const;
        VkQueue get_transfer() const;
        VkQueue get_present()  const;
      private:
        std::vector<VkQueue> queues;
    };
  }
}

#endif // _HSJE_VULKAN_QUEUE_FAMILY_HPP__

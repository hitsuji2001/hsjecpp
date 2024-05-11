#ifndef _HSJE_VULKAN_QUEUE_FAMILY_HPP__
#define _HSJE_VULKAN_QUEUE_FAMILY_HPP__

#include "common.hpp"
#include "queue_family_index.hpp"

namespace hsje {
namespace vk {
class QueueFamily {
 public:
  virtual ~QueueFamily() = default;
  QueueFamily(const std::shared_ptr<vk::Devices>& logical_device,
              const QueueFamilyIndex& indices);

  VkQueue get_graphic() const;
  VkQueue get_compute() const;
  VkQueue get_transfer() const;
  VkQueue get_present() const;

  std::unordered_map<QueueFamilyType, VkQueue> get() const;

 private:
  std::unordered_map<QueueFamilyType, VkQueue> queues;
};
}  // namespace vk
}  // namespace hsje

#endif  // _HSJE_VULKAN_QUEUE_FAMILY_HPP__

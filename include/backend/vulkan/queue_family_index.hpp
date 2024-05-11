#ifndef _HSJE_VULKAN_QUEUE_FAMILY_INDEX_HPP__
#define _HSJE_VULKAN_QUEUE_FAMILY_INDEX_HPP__

#include <unordered_map>

#include "common.hpp"
#include "devices.hpp"
#include "surface.hpp"

namespace hsje {
namespace vk {
enum class QueueFamilyType { Graphic, Compute, Transfer, Present, Size };

class QueueFamilyIndex {
 public:
  QueueFamilyIndex(const VkPhysicalDevice& device);
  QueueFamilyIndex(const std::shared_ptr<vk::PhysicalDevice>& device,
                   const std::shared_ptr<vk::Surface>& surface);
  virtual ~QueueFamilyIndex() = default;

  bool is_complete() const;
  bool has_graphic() const;
  bool has_compute() const;
  bool has_transfer() const;
  bool has_present() const;

  uint32_t get_graphic() const;
  uint32_t get_compute() const;
  uint32_t get_transfer() const;
  uint32_t get_present() const;

  std::unordered_map<QueueFamilyType, uint32_t> get() const;

 private:
  void choose_gracomtra(const VkPhysicalDevice& device);
  std::vector<VkQueueFamilyProperties> get_queue_families(
      const VkPhysicalDevice& device) const;

 private:
  std::unordered_map<QueueFamilyType, uint32_t> indices;
  std::shared_ptr<vk::PhysicalDevice> device;
  std::shared_ptr<vk::Surface> surface;
  static const uint32_t _not_present;
};
}  // namespace vk
}  // namespace hsje

#endif  // _HSJE_VULKAN_QUEUE_FAMILY_INDEX_HPP__

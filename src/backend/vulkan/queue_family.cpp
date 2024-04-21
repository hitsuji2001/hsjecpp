#include "backend/vulkan/queue_family.hpp"

namespace hsje {
  const uint32_t QueueFamilyIndex::_not_present = (uint32_t)-1;

  QueueFamilyIndex::QueueFamilyIndex(const VkPhysicalDevice& device) {
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, queue_families.data());

    uint8_t min_transfer_score = (uint8_t) -1;
    for (size_t i = 0; i < queue_families.size(); i++) {
      uint8_t current_transfer_score = 0;
      const auto& qf = queue_families.at(i);
      if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        this->graphic = i;
        current_transfer_score++;
      } 
      if (qf.queueFlags & VK_QUEUE_COMPUTE_BIT) {
        this->compute = i;
        current_transfer_score++;
      } 
      if (qf.queueFlags & VK_QUEUE_TRANSFER_BIT) {
        // Try to find dedicated transfer queue
        if (current_transfer_score <= min_transfer_score) {
          min_transfer_score = current_transfer_score;
          this->transfer = i;
        }
      }
    }
  }

  QueueFamilyIndex::~QueueFamilyIndex() {}
  uint32_t QueueFamilyIndex::get_graphic()  const { return this->graphic; }
  uint32_t QueueFamilyIndex::get_compute()  const { return this->compute; }
  uint32_t QueueFamilyIndex::get_transfer() const { return this->transfer; }
  uint32_t QueueFamilyIndex::get_present()  const { return this->present; }

  bool QueueFamilyIndex::has_graphic()  const { return this->graphic  != QueueFamilyIndex::_not_present; }
  bool QueueFamilyIndex::has_compute()  const { return this->compute  != QueueFamilyIndex::_not_present; }
  bool QueueFamilyIndex::has_transfer() const { return this->transfer != QueueFamilyIndex::_not_present; }
  bool QueueFamilyIndex::has_present()  const { return this->present  != QueueFamilyIndex::_not_present; }
}

#include "backend/vulkan/queue_family_index.hpp"

namespace hsje {
  const uint32_t QueueFamilyIndex::_not_present = static_cast<uint32_t>(-1);

  QueueFamilyIndex::QueueFamilyIndex(const VkPhysicalDevice& device) {
    this->indices.reserve(static_cast<size_t>(QueueFamilyType::Size));
    for (size_t i = 0; i < this->indices.size(); ++i) {
      this->indices.at(i) = QueueFamilyIndex::_not_present;
    }

    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, queue_families.data());

    uint8_t min_transfer_score = static_cast<uint8_t>(-1);
    for (size_t i = 0; i < queue_families.size(); i++) {
      uint8_t current_transfer_score = 0;
      const auto& qf = queue_families.at(i);
      if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        this->indices[static_cast<size_t>(QueueFamilyType::Graphic)] = i;
        current_transfer_score++;
      } 
      if (qf.queueFlags & VK_QUEUE_COMPUTE_BIT) {
        this->indices[static_cast<size_t>(QueueFamilyType::Compute)] = i;
        current_transfer_score++;
      } 
      if (qf.queueFlags & VK_QUEUE_TRANSFER_BIT) {
        // Try to find dedicated transfer queue
        if (current_transfer_score <= min_transfer_score) {
          min_transfer_score = current_transfer_score;
          this->indices[static_cast<size_t>(QueueFamilyType::Transfer)] = i;
        }
      }
    }
  }

  QueueFamilyIndex::~QueueFamilyIndex() {}
  uint32_t QueueFamilyIndex::get_graphic() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Graphic)]; 
  }

  uint32_t QueueFamilyIndex::get_compute() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Compute)]; 
  }

  uint32_t QueueFamilyIndex::get_transfer() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Transfer)]; 
  }

  uint32_t QueueFamilyIndex::get_present() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Present)]; 
  }

  bool QueueFamilyIndex::has_graphic() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Graphic)] !=
      QueueFamilyIndex::_not_present;
  }

  bool QueueFamilyIndex::has_compute() const { 
    return this->indices[static_cast<size_t>(QueueFamilyType::Compute)] != 
      QueueFamilyIndex::_not_present;
  }

  bool QueueFamilyIndex::has_transfer() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Transfer)] !=
      QueueFamilyIndex::_not_present; 
  }

  bool QueueFamilyIndex::has_present() const {
    return this->indices[static_cast<size_t>(QueueFamilyType::Present)] !=
      QueueFamilyIndex::_not_present; 
  }
}

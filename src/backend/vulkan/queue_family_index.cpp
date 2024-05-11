#include "backend/vulkan/queue_family_index.hpp"

namespace hsje {
namespace vk {
const uint32_t QueueFamilyIndex::_not_present = static_cast<uint32_t>(-1);

QueueFamilyIndex::QueueFamilyIndex(const VkPhysicalDevice& device) {
  this->choose_gracomtra(device);
}

QueueFamilyIndex::QueueFamilyIndex(
    const std::shared_ptr<vk::PhysicalDevice>& _device,
    const std::shared_ptr<vk::Surface>& _surface)
    : device(_device), surface(_surface) {
  for (size_t i = 0; i < static_cast<size_t>(QueueFamilyType::Size); ++i) {
    this->indices[QueueFamilyType(i)] = QueueFamilyIndex::_not_present;
  }
  const auto& queue_families = this->get_queue_families(this->device->get());
  uint32_t min_transfer_score = static_cast<uint32_t>(-1);
  for (size_t i = 0; i < queue_families.size(); i++) {
    uint32_t current_transfer_score = 0;
    const auto& qf = queue_families.at(i);
    if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      this->indices[QueueFamilyType::Graphic] = i;
      current_transfer_score++;
    }
    if (qf.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      this->indices[QueueFamilyType::Compute] = i;
      current_transfer_score++;
    }
    if (qf.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      // Try to find dedicated transfer queue
      if (current_transfer_score <= min_transfer_score) {
        min_transfer_score = current_transfer_score;
        this->indices[QueueFamilyType::Transfer] = i;
      }
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        this->device->get(), i, this->surface->get(), &present_support);
    if (present_support) this->indices[QueueFamilyType::Present] = i;
    if (this->is_complete()) return;
  }
  throw std::runtime_error(
      "Couldn't find device with present and graphic familes.");
}

void QueueFamilyIndex::choose_gracomtra(const VkPhysicalDevice& device) {
  for (size_t i = 0; i < static_cast<size_t>(QueueFamilyType::Size); ++i) {
    this->indices[QueueFamilyType(i)] = QueueFamilyIndex::_not_present;
  }
  const auto& queue_families = this->get_queue_families(device);

  uint32_t min_transfer_score = static_cast<uint32_t>(-1);
  for (size_t i = 0; i < queue_families.size(); i++) {
    uint32_t current_transfer_score = 0;
    const auto& qf = queue_families.at(i);
    if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      this->indices[QueueFamilyType::Graphic] = i;
      current_transfer_score++;
    }
    if (qf.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      this->indices[QueueFamilyType::Compute] = i;
      current_transfer_score++;
    }
    if (qf.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      // Try to find dedicated transfer queue
      if (current_transfer_score <= min_transfer_score) {
        min_transfer_score = current_transfer_score;
        this->indices[QueueFamilyType::Transfer] = i;
      }
    }
  }
}

std::vector<VkQueueFamilyProperties> QueueFamilyIndex::get_queue_families(
    const VkPhysicalDevice& device) const {
  uint32_t family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count,
                                           queue_families.data());

  return queue_families;
}

uint32_t QueueFamilyIndex::get_graphic() const {
  try {
    return this->indices.at(QueueFamilyType::Graphic);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

uint32_t QueueFamilyIndex::get_compute() const {
  try {
    return this->indices.at(QueueFamilyType::Compute);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

uint32_t QueueFamilyIndex::get_transfer() const {
  try {
    return this->indices.at(QueueFamilyType::Transfer);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

uint32_t QueueFamilyIndex::get_present() const {
  try {
    return this->indices.at(QueueFamilyType::Present);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

bool QueueFamilyIndex::is_complete() const {
  return this->has_graphic() && this->has_present();
}

bool QueueFamilyIndex::has_graphic() const {
  try {
    if (this->indices.at(QueueFamilyType::Graphic) !=
        QueueFamilyIndex::_not_present)
      return true;
    else
      return false;
  } catch (const std::out_of_range& e) {
    return false;
  }
}

bool QueueFamilyIndex::has_compute() const {
  try {
    if (this->indices.at(QueueFamilyType::Compute) !=
        QueueFamilyIndex::_not_present)
      return true;
    else
      return false;
  } catch (const std::out_of_range& e) {
    return false;
  }
}

bool QueueFamilyIndex::has_transfer() const {
  try {
    if (this->indices.at(QueueFamilyType::Transfer) !=
        QueueFamilyIndex::_not_present)
      return true;
    else
      return false;
  } catch (const std::out_of_range& e) {
    return false;
  }
}

bool QueueFamilyIndex::has_present() const {
  try {
    if (this->indices.at(QueueFamilyType::Present) !=
        QueueFamilyIndex::_not_present)
      return true;
    else
      return false;
  } catch (const std::out_of_range& e) {
    return false;
  }
}

std::unordered_map<QueueFamilyType, uint32_t> QueueFamilyIndex::get() const {
  return this->indices;
}
}  // namespace vk
}  // namespace hsje

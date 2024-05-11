#include "backend/vulkan/queue_family.hpp"

using namespace hlog;

namespace hsje {
namespace vk {
QueueFamily::QueueFamily(const std::shared_ptr<vk::Devices>& dev,
                         const QueueFamilyIndex& indices) {
  log::debug("Creating Queue Families...");
  for (size_t i = 0; i < static_cast<size_t>(QueueFamilyType::Size); ++i) {
    this->queues[QueueFamilyType(i)] = nullptr;
  }
  if (indices.has_graphic()) {
    vkGetDeviceQueue(dev->get_logical()->get(), indices.get_graphic(), 0,
                     &this->queues[QueueFamilyType::Graphic]);
  }
  if (indices.has_compute()) {
    vkGetDeviceQueue(dev->get_logical()->get(), indices.get_compute(), 0,
                     &this->queues[QueueFamilyType::Compute]);
  }
  if (indices.has_transfer()) {
    vkGetDeviceQueue(dev->get_logical()->get(), indices.get_transfer(), 0,
                     &this->queues[QueueFamilyType::Transfer]);
  }
  if (indices.has_present()) {
    vkGetDeviceQueue(dev->get_logical()->get(), indices.get_present(), 0,
                     &this->queues[QueueFamilyType::Present]);
  }
  log::info("Successfully created all queue families!");
}

VkQueue QueueFamily::get_graphic() const {
  try {
    return this->queues.at(QueueFamilyType::Graphic);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

VkQueue QueueFamily::get_compute() const {
  try {
    return this->queues.at(QueueFamilyType::Compute);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

VkQueue QueueFamily::get_transfer() const {
  try {
    return this->queues.at(QueueFamilyType::Transfer);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

VkQueue QueueFamily::get_present() const {
  try {
    return this->queues.at(QueueFamilyType::Present);
  } catch (const std::out_of_range& e) {
    throw e;
  }
}

std::unordered_map<QueueFamilyType, VkQueue> QueueFamily::get() const {
  return this->queues;
}
}  // namespace vk
}  // namespace hsje

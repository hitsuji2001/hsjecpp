#include "backend/vulkan/queue_family.hpp"

using namespace hlog;

namespace hsje {
  namespace vk {
    QueueFamily::QueueFamily(
        const VkDevice& logical_device,
        const QueueFamilyIndex& indices
    ) {
      log::debug("Creating Queue Families...");
      this->queues.reserve(static_cast<size_t>(QueueFamilyType::Size));
      if (indices.has_graphic()) {
        vkGetDeviceQueue(
            logical_device,
            indices.get_graphic(),
            0,
            &this->queues[static_cast<size_t>(QueueFamilyType::Graphic)]
        );
      }
      if (indices.has_compute()) {
        vkGetDeviceQueue(
            logical_device,
            indices.get_compute(),
            0,
            &this->queues[static_cast<size_t>(QueueFamilyType::Compute)]
        );
      }
      if (indices.has_transfer()) {
        vkGetDeviceQueue(
            logical_device,
            indices.get_transfer(),
            0,
            &this->queues[static_cast<size_t>(QueueFamilyType::Transfer)]
        );
      }
      if (indices.get_present()) {
        vkGetDeviceQueue(
            logical_device,
            indices.get_present(),
            0,
            &this->queues[static_cast<size_t>(QueueFamilyType::Present)]
        );
      }
      log::info("Successfully created all queue families!");
    }

    QueueFamily::QueueFamily() {}
    QueueFamily::~QueueFamily() {}

    VkQueue QueueFamily::get_graphic()  const { return this->queues[(size_t)QueueFamilyType::Graphic]; }
    VkQueue QueueFamily::get_compute()  const { return this->queues[(size_t)QueueFamilyType::Compute]; }
    VkQueue QueueFamily::get_transfer() const { return this->queues[(size_t)QueueFamilyType::Transfer]; }
    VkQueue QueueFamily::get_present()  const { return this->queues[(size_t)QueueFamilyType::Present]; }
  }
}

#include "backend/vulkan/surface.hpp"

namespace hsje {
namespace vk {
Surface::Surface(const std::shared_ptr<Instance>& inst,
                 const std::shared_ptr<Window>& win)
    : instance(inst), window(win) {
  if (!this->window->create_window_surface(this->instance.get()->get(), nullptr,
                                           nullptr, &this->surface)) {
    throw std::runtime_error("Failed to create Window Surface!");
  }
}

Surface::~Surface() {
  vkDestroySurfaceKHR(this->instance->get(), this->surface, nullptr);
}

VkSurfaceKHR Surface::get() const { return this->surface; }
}  // namespace vk
}  // namespace hsje

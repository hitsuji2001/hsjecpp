#ifndef _HSJE_VULKAN_SURFACE_HPP__
#define _HSJE_VULKAN_SURFACE_HPP__

#include "common.hpp"
#include "instance.hpp"
#include "window/window.hpp"

namespace hsje {
namespace vk {
class Surface {
 public:
  Surface(const std::shared_ptr<Instance>& instance,
          const std::shared_ptr<Window>& window);
  virtual ~Surface();
  VkSurfaceKHR get() const;

 private:
  VkSurfaceKHR surface;
  std::shared_ptr<Instance> instance;
  std::shared_ptr<Window> window;
};
}  // namespace vk
}  // namespace hsje

#endif  // _HSJE_VULKAN_SURFACE_HPP__

#ifndef _GLFW_WINDOW_HPP__
#define _GLFW_WINDOW_HPP__

#include "core/hlog.hpp"
#include "window.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace hsje {
class GlfwWindow : public Window {
 public:
  GlfwWindow();
  GlfwWindow(uint32_t w, uint32_t h, const std::string& title);
  virtual ~GlfwWindow();

 public:
  void init() override;
  void poll_events() override;
  void wait_events() override;
  bool should_close() override;
  bool create_window_surface(void* instance, void* window,
                             void* allocation_callback, void* surface) override;
  std::vector<const char*> get_require_instance_extensions() override;

 private:
  GLFWwindow* window;
};
}  // namespace hsje

#endif  // _GLFW_WINDOW_HPP__

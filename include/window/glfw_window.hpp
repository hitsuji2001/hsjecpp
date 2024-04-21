#ifndef _GLFW_WINDOW_HPP__
#define _GLFW_WINDOW_HPP__

#include "core/hlog.hpp"
#include "window.hpp"

namespace hsje {
  class GlfwWindow: public Window {
    public:
      using Window::Window;
      virtual ~GlfwWindow();

    public:
      void init() override;
      void poll_events() override;
      void wait_events() override;
      bool should_close() override;
      std::vector<const char*> get_require_instance_extensions() override;

    private:
      GLFWwindow *window;
  };
}

#endif // _GLFW_WINDOW_HPP__

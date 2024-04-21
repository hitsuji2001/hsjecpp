#include "window/glfw_window.hpp"

using namespace hlog;

namespace hsje {
  GlfwWindow::~GlfwWindow() {
    log::debug("Destroying GLFW window...");
    glfwDestroyWindow(this->window);
    log::debug("GLFW Terminating...");
    glfwTerminate();
  }

  void GlfwWindow::init() {
    log::debug("Initializing GLFW...");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (this->get_width() == 0) this->set_width(800);
    if (this->get_height() == 0) this->set_height(800);
    if (this->get_title().empty()) this->set_title("hsjengine");

    log::debug(
        "Creating window `{}` with size `{}x{}`",
        this->get_title(),
        this->get_width(),
        this->get_height()
    );
    this->window = glfwCreateWindow(
          this->get_width(),
          this->get_height(),
          this->get_title().c_str(),
          nullptr,
          nullptr
    );
    if (this->window == nullptr) {
      throw std::runtime_error("Could not create window");
    }
    log::info(
        "Successfully created window `{}` with size `{}x{}`",
        this->get_title(),
        this->get_width(),
        this->get_height()
    );
  }

  void GlfwWindow::poll_events() { glfwPollEvents(); }
  void GlfwWindow::wait_events() { glfwWaitEvents(); }
  bool GlfwWindow::should_close() { return glfwWindowShouldClose(this->window); }

  std::vector<const char*> GlfwWindow::get_require_instance_extensions() {
    log::debug("Checking if minimal functional ICD Vulkan loader...");
    if (!glfwVulkanSupported()) {
      throw std::runtime_error("Could not find minimal functional ICD Vulkan loader");
    }
    log::info("Functional ICD Vulkan loader supported!");

    log::trace("Looking for extensions...");
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions  = nullptr;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    log::trace("Found required extensions:");
    std::vector<const char*> extensions;
    for (size_t i = 0; i < glfw_extension_count; ++i) {
      extensions.emplace_back(glfw_extensions[i]);
      log::trace("\t{}", glfw_extensions[i]);
    }

    return extensions;
  }
}

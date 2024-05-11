#include "window/glfw_window.hpp"

using namespace hlog;

namespace hsje {
GlfwWindow::~GlfwWindow() {
  log::debug("Destroying GLFW window...");
  glfwDestroyWindow(this->window);
  log::debug("GLFW Terminating...");
  glfwTerminate();
}

GlfwWindow::GlfwWindow() {
  this->width = 800;
  this->height = 600;
  this->title = "hsje";
  this->init();
}

GlfwWindow::GlfwWindow(uint32_t w, uint32_t h, const std::string& t) {
  this->width = w;
  this->height = h;
  this->title = t;
  this->init();
}

void GlfwWindow::init() {
  log::debug("Initializing GLFW...");
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  log::debug("Creating window `{}` with size `{}x{}`...", this->title,
             this->width, this->height);
  this->window = glfwCreateWindow(this->width, this->height,
                                  this->title.c_str(), nullptr, nullptr);
  if (this->window == nullptr) {
    throw std::runtime_error("Could not create window");
  }
  log::info("Successfully created window `{}` with size `{}x{}`", this->title,
            this->width, this->height);
}

void GlfwWindow::poll_events() { glfwPollEvents(); }
void GlfwWindow::wait_events() { glfwWaitEvents(); }
bool GlfwWindow::should_close() { return glfwWindowShouldClose(this->window); }

bool GlfwWindow::create_window_surface(void* instance, void* window,
                                       void* allocation_callback,
                                       void* surface) {
  if (window == nullptr) window = this->window;
  VkResult res =
      glfwCreateWindowSurface((VkInstance)instance, (GLFWwindow*)window,
                              (VkAllocationCallbacks*)allocation_callback,
                              (VkSurfaceKHR*)&(*(VkSurfaceKHR)surface));
  return res == VK_SUCCESS;
}

std::vector<const char*> GlfwWindow::get_require_instance_extensions() {
  log::debug("Checking if minimal functional ICD Vulkan loader...");
  if (!glfwVulkanSupported()) {
    throw std::runtime_error(
        "Could not find minimal functional ICD Vulkan loader");
  }
  log::info("Functional ICD Vulkan loader supported!");

  log::trace("Looking for extensions...");
  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = nullptr;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  log::trace("Found required extensions:");
  std::vector<const char*> extensions;
  for (size_t i = 0; i < glfw_extension_count; ++i) {
    extensions.emplace_back(glfw_extensions[i]);
    log::trace("\t{}", glfw_extensions[i]);
  }

  return extensions;
}
}  // namespace hsje

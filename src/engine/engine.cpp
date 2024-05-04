#include "engine/engine.hpp"
#include "backend/vulkan/vulkan_backend.hpp"

using namespace hlog;

namespace hsje {
  void Engine::run() {
    this->init();
    this->main_loop();
    this->cleanup();
  }

  void Engine::init() { 
    this->backend = std::make_shared<VulkanBackend>();
  }

  void Engine::cleanup() {
  }

  void Engine::main_loop() { 
    // while (!this->backend->get_window()->should_close()) {
    //   this->backend->get_window()->poll_events(); 
    // }
  }
}

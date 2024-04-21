#include "engine/engine.hpp"

using namespace hlog;

namespace hsje {
  void Engine::run() {
    this->backend = std::make_shared<VulkanBackend>();

    this->init();
    this->main_loop();
    this->cleanup();
  }

  void Engine::init() { this->backend->init(); }
  void Engine::cleanup() { this->backend->cleanup(); }
  void Engine::main_loop() { 
    while (!this->backend->get_window()->should_close()) {
    this->backend->get_window()->poll_events(); 
    }
  }
}

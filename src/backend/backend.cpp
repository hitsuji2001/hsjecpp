#include "backend/backend.hpp"

namespace hsje {
  Backend::Backend() {}
  Backend::~Backend() {}

  std::shared_ptr<Window> Backend::get_window() const { 
    return this->window; 
  }

  void Backend::set_window(const std::shared_ptr<Window>& w) {
    this->window = w;
  }
}

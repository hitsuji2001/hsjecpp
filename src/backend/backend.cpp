#include "backend/backend.hpp"

namespace hsje {
  std::shared_ptr<Window> Backend::get_window() const { 
    return this->window; 
  }
}

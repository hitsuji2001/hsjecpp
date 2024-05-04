#ifndef _HSJE_BACKEND_HPP__
#define _HSJE_BACKEND_HPP__

#include <memory>

#include "window/window.hpp"

namespace hsje {
  class Backend {
    public:
      Backend() = default;
      virtual ~Backend()     = default;
      std::shared_ptr<Window> get_window() const;

    protected:
      std::shared_ptr<Window> window;
  };
};

#endif // _HSJE_BACKEND_HPP__

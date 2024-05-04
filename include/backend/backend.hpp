#ifndef _HSJE_BACKEND_HPP__
#define _HSJE_BACKEND_HPP__

#include <memory>

#include "window/window.hpp"

namespace hsje {
  class Backend {
    public:
      Backend() = default;

      std::shared_ptr<Window> get_window() const;
    public:
      virtual ~Backend()     = default;
      // virtual void init()    = 0;
      // virtual void cleanup() = 0;

    protected:
      std::shared_ptr<Window> window;
  };
};

#endif // _HSJE_BACKEND_HPP__

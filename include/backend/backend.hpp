#ifndef _HSJE_BACKEND_HPP__
#define _HSJE_BACKEND_HPP__

#include <memory>

#include "window/window.hpp"

namespace hsje {
  class Backend {
    public:
      Backend();

      std::shared_ptr<Window> get_window() const;
      void set_window(const std::shared_ptr<Window>& w);
    public:
      virtual ~Backend();
      virtual void init() = 0;
      virtual void cleanup() = 0;

    private:
      std::shared_ptr<Window> window;
  };
};

#endif // _HSJE_BACKEND_HPP__

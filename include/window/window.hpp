#ifndef _HSJE_WINDOW_HPP__
#define _HSJE_WINDOW_HPP__

#include <vector>
#include <string>
#include <cstdint>

#include <GLFW/glfw3.h>

namespace hsje {
  class Window {
    public:
      Window();
      Window(uint32_t w, uint32_t h, const std::string& title);

      uint32_t    get_width()  const;
      uint32_t    get_height() const;
      std::string get_title()  const;

      void        set_width(uint32_t w);
      void        set_height(uint32_t h);
      void        set_title(const std::string& title);

    public:
      virtual ~Window();
      virtual void init() = 0;
      virtual void poll_events() = 0;
      virtual void wait_events() = 0;
      virtual bool should_close() = 0;
      virtual std::vector<const char*> get_require_instance_extensions() = 0;

    private:
      uint32_t    width;
      uint32_t    height;
      std::string title;
  };
};

#endif // _HSJE_WINDOW_HPP__


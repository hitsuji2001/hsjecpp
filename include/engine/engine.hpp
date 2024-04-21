#ifndef _HSJE_ENGINE_HPP__
#define _HSJE_ENGINE_HPP__

#include "core/hlog.hpp"
#include "backend/vulkan/vulkan_backend.hpp"

namespace hsje {
  class Engine {
    public:
      void run();

    private:
      void init();
      void main_loop();
      void cleanup();

    private:
      std::shared_ptr<Backend> backend;
  };
};

#endif // _HSJE_ENGINE_HPP__

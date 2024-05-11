#ifndef _HSJE_ENGINE_HPP__
#define _HSJE_ENGINE_HPP__

#include "backend/backend.hpp"
#include "core/hlog.hpp"

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
};  // namespace hsje

#endif  // _HSJE_ENGINE_HPP__

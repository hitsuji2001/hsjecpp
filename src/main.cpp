#include <iostream>

#include "engine/engine.hpp"

int main(void) {
  hsje::Engine engine;

  try { 
    engine.run();
  } catch (const std::exception& e) {
    hlog::log::error(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

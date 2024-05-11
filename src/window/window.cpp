#include "window/window.hpp"

namespace hsje {
Window::Window(uint32_t w, uint32_t h, const std::string& t)
    : width(w), height(h), title(t) {}

uint32_t Window::get_width() const { return this->width; }
uint32_t Window::get_height() const { return this->height; }
std::string Window::get_title() const { return this->title; }

void Window::set_width(uint32_t w) { this->width = w; }
void Window::set_height(uint32_t h) { this->height = h; }
void Window::set_title(const std::string& title) { this->title = title; }
}  // namespace hsje

#ifndef _HLOG_HPP__
#define _HLOG_HPP__

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <unordered_map>

namespace hlog {
  enum class Severity { Fatal, Error, Warn, Info, Debug, Trace, Count };

  class log {
    public:
      template<typename... T>
      static void fatal(fmt::format_string<T...> format, T&&... args) {
        _print_log(Severity::Fatal, format, std::forward<T>(args)...);
      }

      template<typename... T>
      static void error(fmt::format_string<T...> format, T&&... args) {
        _print_log(Severity::Error, format, std::forward<T>(args)...);
      }

      template<typename... T>
      static void warn(fmt::format_string<T...> format, T&&... args) {
        _print_log(Severity::Warn, format, std::forward<T>(args)...);
      }

      template<typename... T>
      static void info(fmt::format_string<T...> format, T&&... args) {
        _print_log(Severity::Info, format, std::forward<T>(args)...);
      }

      template<typename... T>
      static void debug(fmt::format_string<T...> format, T&&... args) {
        _print_log(Severity::Debug, format, std::forward<T>(args)...);
      }

      template<typename... T>
      static void trace(fmt::format_string<T...> format, T&&... args) {
        _print_log(Severity::Trace, format, std::forward<T>(args)...);
      }

      static void set_log_level(const Severity& severity);

    private:
      // https://github.com/fmtlib/fmt/issues/2391
      template<typename... T>
      static void _print_log(Severity severity, fmt::format_string<T...> format, T&&... args) {
        if (severity > _severity_min) return;
        const auto& message = fmt::format(format, std::forward<T>(args)...);
        const auto& now     = std::chrono::system_clock::now();

        fmt::print(fg(fmt::color::gray) | fmt::emphasis::faint, "[{}]", now);
        fmt::print(" ");
        fmt::print(_severity_text_styles.at(severity), "[{:^5}]", _severity_names.at(severity));
        fmt::print(" ");
        fmt::print(_severity_message_styles.at(severity), message);
        fmt::print("\n");
      }

    private:
      static Severity                                                  _severity_min;
      static const std::unordered_map<Severity, const std::string>     _severity_names;
      static const std::unordered_map<Severity, const fmt::text_style> _severity_base_color;
      static const std::unordered_map<Severity, const fmt::text_style> _severity_text_styles;
      static const std::unordered_map<Severity, const fmt::text_style> _severity_message_styles;
  };
}

#endif // _HLOG_HPP__

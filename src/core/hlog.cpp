#include "core/hlog.hpp"

namespace hlog {
#ifdef _DEBUG
  Severity log::_severity_min = Severity::Trace;
#else
  Severity log::_severity_min = Severity::Error;
#endif

  const char* log::_severity_names[(size_t)Severity::Count] = {
    "Fatal",
    "Error",
    "Warn",
    "Info",
    "Debug",
    "Trace"
  };

  const fmt::text_style log::_severity_base_color[(size_t)Severity::Count] = {
    fg(fmt::color::red),
    fg(fmt::color::red),
    fg(fmt::color::golden_rod),
    fg(fmt::color::sky_blue),
    fg(fmt::color::honey_dew),
    fg(fmt::color::dim_gray)
  };

  const fmt::text_style log::_severity_text_styles[(size_t)Severity::Count] = {
    log::_severity_base_color[(size_t)Severity::Fatal] | fmt::emphasis::bold | bg(fmt::color::yellow),
    log::_severity_base_color[(size_t)Severity::Error] | fmt::emphasis::bold,
    log::_severity_base_color[(size_t)Severity::Warn]  | fmt::emphasis::bold,
    log::_severity_base_color[(size_t)Severity::Info]  | fmt::emphasis::bold,
    log::_severity_base_color[(size_t)Severity::Debug] | fmt::emphasis::bold,
    log::_severity_base_color[(size_t)Severity::Trace] | fmt::emphasis::bold
  };

  const fmt::text_style log::_severity_message_styles[(size_t)Severity::Count] = {
    log::_severity_base_color[(size_t)Severity::Fatal] | fmt::emphasis::bold,
    log::_severity_base_color[(size_t)Severity::Error] | fmt::emphasis::faint,
    log::_severity_base_color[(size_t)Severity::Warn]  | fmt::emphasis::faint,
    log::_severity_base_color[(size_t)Severity::Info]  | fmt::emphasis::faint,
    log::_severity_base_color[(size_t)Severity::Debug] | fmt::emphasis::faint,
    log::_severity_base_color[(size_t)Severity::Trace] | fmt::emphasis::faint
  };

  void log::set_log_level(const Severity& severity) {
    log::_severity_min = severity;
  }
}

#include "core/hlog.hpp"

namespace hlog {
#ifdef _DEBUG
  Severity log::_severity_min = Severity::Trace;
#else
  Severity log::_severity_min = Severity::Error;
#endif

  const std::unordered_map<Severity, const std::string> log::_severity_names = {
    { Severity::Fatal, "Fatal" },
    { Severity::Error, "Error" },
    { Severity::Warn,  "Warn"  },
    { Severity::Info,  "Info"  },
    { Severity::Debug, "Debug" },
    { Severity::Trace, "Trace" }
  };

  const std::unordered_map<Severity, const fmt::text_style> log::_severity_base_color = {
    { Severity::Fatal, fg(fmt::color::red)        },
    { Severity::Error, fg(fmt::color::red)        },
    { Severity::Warn,  fg(fmt::color::golden_rod) },
    { Severity::Info,  fg(fmt::color::sky_blue)   },
    { Severity::Debug, fg(fmt::color::honey_dew)  },
    { Severity::Trace, fg(fmt::color::dim_gray)   }
  };

  const std::unordered_map<Severity, const fmt::text_style> log::_severity_text_styles = {
    { Severity::Fatal, log::_severity_base_color.at(Severity::Fatal) | fmt::emphasis::bold | bg(fmt::color::yellow) },
    { Severity::Error, log::_severity_base_color.at(Severity::Error) | fmt::emphasis::bold                          },
    { Severity::Warn,  log::_severity_base_color.at(Severity::Warn)  | fmt::emphasis::bold                          },
    { Severity::Info,  log::_severity_base_color.at(Severity::Info)  | fmt::emphasis::bold                          },
    { Severity::Debug, log::_severity_base_color.at(Severity::Debug) | fmt::emphasis::bold                          },
    { Severity::Trace, log::_severity_base_color.at(Severity::Trace) | fmt::emphasis::bold                          }
  };

  const std::unordered_map<Severity, const fmt::text_style> log::_severity_message_styles = {
    { Severity::Fatal, log::_severity_base_color.at(Severity::Fatal) | fmt::emphasis::bold  },
    { Severity::Error, log::_severity_base_color.at(Severity::Error) | fmt::emphasis::faint },
    { Severity::Warn,  log::_severity_base_color.at(Severity::Warn)  | fmt::emphasis::faint },
    { Severity::Info,  log::_severity_base_color.at(Severity::Info)  | fmt::emphasis::faint },
    { Severity::Debug, log::_severity_base_color.at(Severity::Debug) | fmt::emphasis::faint },
    { Severity::Trace, log::_severity_base_color.at(Severity::Trace) | fmt::emphasis::faint }
  };

  void log::set_log_level(const Severity& severity) {
    log::_severity_min = severity;
  }
}

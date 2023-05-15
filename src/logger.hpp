#pragma once

#include <map>
#include <iostream>
#include <iomanip>
#include <source_location>
#include <fmt/format.h>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

namespace kiq::log {

enum class custom_levels
{
  error = 600,
  trace = 700
};
//-------------------------------------------------
enum colour
{
  YELLOW   = 93,
  RED      = 31,
  GREEN    = 38,
  WHITE    = 97,
  MAGENTA  = 35,
  TURQOISE = 36
};
//-------------------------------------------------
colour to_colour(const LEVELS level);
//-------------------------------------------------
constexpr const int custom_error = static_cast<int>(custom_levels::error);
constexpr const int custom_trace = static_cast<int>(custom_levels::trace);
//-------------------------------------------------
static const LEVELS ERROR { custom_error, {"ERROR"} };
static const LEVELS TRACE { custom_trace, {"TRACE"} };
//-------------------------------------------------
struct coloursink
{
  void write(g3::LogMessageMover log) const;
};
//-------------------------------------------------
struct filesink
{
  void write(g3::LogMessageMover) const;
};
//-------------------------------------------------
static const char* default_log_level = "info";
namespace
{
  std::unique_ptr<g3::LogWorker> lg_;
}
//-------------------------------------------------
enum class loglevel
{
  silent = 0x00,
  fatal  = g3::kFatalValue,
  error  = custom_error,
  warn   = g3::kWarningValue,
  info   = g3::kInfoValue,
  debug  = g3::kDebugValue,
  trace  = custom_trace
};
//-------------------------------------------------
using loglevel_t = std::map<std::string, loglevel>;
static const loglevel_t log_level
{
  {"trace",    loglevel::trace  },
  {"debug",    loglevel::debug  },
  {"info",     loglevel::info   },
  {"warn",     loglevel::warn   },
  {"error",    loglevel::error  },
  {"fatal",    loglevel::fatal  },
  {"silent",   loglevel::silent }
};
//-------------------------------------------------
struct fmt_loc
{
  const char*          value_;
  std::source_location loc_;

  fmt_loc(const char* s, const std::source_location& l = std::source_location::current())
  : value_(s),
    loc_(l) {}
};
//-------------------------------------------------
static std::string func_name(const std::source_location& loc)
{
  std::string full = loc.function_name();
  const auto  beg  = full.find_first_of(' ') + 1;
  return full.substr(beg, beg);
}
//-------------------------------------------------
static void to_std_out(const LEVELS& level, const std::string& message, const std::source_location& loc)
{
  const auto full_file = std::string{loc.file_name()};
  const auto file      = full_file.substr(full_file.find_last_of('/') + 1);
  const auto timestamp = g3::localtime_formatted(g3::to_system_time(
    g3::high_resolution_time_point{std::chrono::system_clock::now()}), g3::internal::date_formatted + " " + g3::internal::time_formatted);

  std::cout <<
    "\033[" << to_colour(level) << "m" << timestamp << " [" << level.text << "]\t[" << file << ":" << loc.line() << " " << func_name(loc) << "()]\t" <<
    message << "\033[m" <<
  std::endl;
}
//-------------------------------------------------
//--------------------klogger----------------------
//-------------------------------------------------
class klogger
{
public:
  klogger(const std::string& name  = "KLOG",
          const std::string& level = default_log_level,
          const std::string& path = "/tmp/");

  template<typename... Args>
  void d(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::debug)
      to_std_out(DEBUG, fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...), format.loc_);
  }
//-------------------------------------------------
  template<typename... Args>
  void w(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::warn)
      to_std_out(WARNING, fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...), format.loc_);
  }
//-------------------------------------------------
  template<typename... Args>
  void i(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::info)
      to_std_out(INFO, fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...), format.loc_);
  }
//-------------------------------------------------
  template<typename... Args>
  void e(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::error)
      to_std_out(ERROR, fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...), format.loc_);
  }
//-------------------------------------------------
  template<typename... Args>
  void f(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::fatal)
      to_std_out(FATAL, fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...), format.loc_);
  }
//-------------------------------------------------
  template<typename... Args>
  void t(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::trace)
      to_std_out(TRACE, fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...), format.loc_);
  }
//-------------------------------------------------
  loglevel get_level() const;
//-------------------------------------------------
  static void    init(const std::string& name, const std::string& level = default_log_level);
  static klogger instance();

private:
  void set_level(loglevel level);

  loglevel      level_;
};
}  // namespace kiq::log

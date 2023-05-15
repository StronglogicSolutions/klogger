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
//--------------------klogger----------------------
//-------------------------------------------------

struct fmt_loc
{
  const char* value_;
  std::source_location loc_;

  fmt_loc(const char* s, const std::source_location& l = std::source_location::current())
  : value_(s),
    loc_(l) {}
};

class klogger {
public:
  klogger(const std::string& name  = "KLOG",
          const std::string& level = default_log_level,
          const std::string& path = "/tmp/");

  template<typename... Args>
  void d(const fmt_loc& format, Args&&... args) const
  {
    if (level_ >= loglevel::debug)
    {
      std::cout << "\033[" << to_colour(DEBUG) << "m" <<
        "2023-05-23 8:57pm" << std::setw(9) << " [DEBUG]\t["  <<
        format.loc_.file_name() << ":" + format.loc_.line() << " " << format.loc_.function_name() << "()]\t" +
        fmt::format(fmt::runtime(format.value_), std::forward<Args>(args)...) << "\033[m" <<
      std::endl;
    }
  }
//-------------------------------------------------
  template<typename... Args>
  void w(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::warn)
      std::cout << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << std::endl;
  }
//-------------------------------------------------
  template<typename... Args>
  void i(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::info)
      std::cout << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << std::endl;
  }
//-------------------------------------------------
  template<typename... Args>
  void e(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::error)
      std::cout << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << std::endl;
  }
//-------------------------------------------------
  template<typename... Args>
  void f(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::fatal)
      std::cout << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << std::endl;
  }
//-------------------------------------------------
  template<typename... Args>
  void t(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::trace)
      std::cout << fmt::format(fmt::runtime(format), std::forward<Args>(args)...) << std::endl;
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

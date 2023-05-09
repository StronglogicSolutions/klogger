#pragma once

#include <map>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <iostream>
#include <source_location>
#include <sstream>
#include <fstream>

namespace kiq::log {

enum class custom_levels
{
  error = 600,
  trace = 700
};
//-------------------------------------------------
constexpr const int custom_error = static_cast<int>(custom_levels::error);
constexpr const int custom_trace = static_cast<int>(custom_levels::trace);
//-------------------------------------------------
static const LEVELS ERROR { custom_error, {"ERROR"} };
static const LEVELS TRACE { custom_trace, {"TRACE"} };
//-------------------------------------------------
struct coloursink
{
  enum colour
  {
    YELLOW   = 93,
    RED      = 31,
    GREEN    = 38,
    WHITE    = 97,
    MAGENTA  = 35,
    TURQOISE = 36
  };

  colour to_colour(const LEVELS level)  const;
  void   write(g3::LogMessageMover log) const;
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
class klogger {
public:
  klogger(const std::string& name  = "KLOG",
          const std::string& level = default_log_level,
          const std::string& path = "/tmp/");
//-------------------------------------------------
//----------------MACRO LOGGER---------------------
//-------------------------------------------------
#define DLOG(...) LOGF(DEBUG,           ##__VA_ARGS__)
#define WLOG(...) LOGF(WARNING,         ##__VA_ARGS__)
#define TLOG(...) LOGF(kiq::log::TRACE, ##__VA_ARGS__)
#define FLOG(...) LOGF(FATAL,           ##__VA_ARGS__)
#define ELOG(...) LOGF(kiq::log::ERROR, ##__VA_ARGS__)
#define KLOG(...) LOGF(INFO,            ##__VA_ARGS__)
//-------------------------------------------------
//----------------CLASS LOGGER---------------------
//-------------------------------------------------
  template<typename... Args>
  void d(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::debug)
      LOGF(DEBUG, format, args...);
  }
//-------------------------------------------------
  template<typename... Args>
  void w(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::warn)
      LOGF(WARNING, format, args...);
  }
//-------------------------------------------------
  template<typename... Args>
  void i(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::info)
      LOGF(INFO, format, args...);
  }
//-------------------------------------------------
  template<typename... Args>
  void e(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::error)
      LOGF(ERROR, format, args...);
  }
//-------------------------------------------------
  template<typename... Args>
  void f(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::fatal)
      LOGF(FATAL, format, args...);
  }
//-------------------------------------------------
  template<typename... Args>
  void t(const char* format, Args&&... args) const
  {
    if (level_ >= loglevel::trace)
      LOGF(TRACE, format, args...);
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

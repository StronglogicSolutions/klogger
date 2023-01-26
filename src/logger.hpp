#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>

#define VLOG SPDLOG_TRACE
#define DLOG SPDLOG_DEBUG
#define KLOG SPDLOG_INFO
#define WLOG SPDLOG_WARN
#define ELOG SPDLOG_ERROR
#define CLOG SPDLOG_CRITICAL

namespace kiq::log {
using log_ptr_t  = std::shared_ptr<spdlog::logger>;
using loglevel_t = std::map<std::string, spdlog::level::level_enum>;

static const loglevel_t log_level{
  {"trace",    spdlog::level::trace},
  {"debug",    spdlog::level::debug},
  {"info",     spdlog::level::info},
  {"warn",     spdlog::level::warn},
  {"error",    spdlog::level::err},
  {"critical", spdlog::level::critical},
  {"off",      spdlog::level::off}
};

static const char* default_log_level = "info";


class klogger {
 public:
  klogger(const std::string&);
  ~klogger();

  static void init(const std::string& level = default_log_level);
  static klogger instance();
};
}  // namespace kiq::log

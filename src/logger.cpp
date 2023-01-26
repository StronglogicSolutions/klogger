#include "logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kiq {
namespace log {
klogger* g_instance;

const std::unordered_map<spdlog::level::level_enum, std::string> LogLevelStrings{
{spdlog::level::trace,    "trace"},
{spdlog::level::debug,    "debug"},
{spdlog::level::info,     "info"},
{spdlog::level::warn,     "warn"},
{spdlog::level::err,      "error"},
{spdlog::level::critical, "critical"},
{spdlog::level::off,      "off"}
};

klogger::klogger(const std::string& logging_level)
{
  using loglevel = spdlog::level::level_enum;
  using sinks_t  = std::vector<spdlog::sink_ptr>;

  static const bool timestamp = true;
  try
  {
    static const char*       path         = "/tmp/klogger.log";
    static const auto        console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    static const auto        file_sink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path);
    static const loglevel    level        = log_level.at(logging_level);
    static const std::string format       = (timestamp) ? "KIQ [%^%-5l%$] - %T.%e - %-20!s :%-4!# - %-22!!%v" :
                                                          "KIQ [%^%-5l%$] - %-20!s :%-4!# - %-22!!%v";
    console_sink->set_level  (level);
    console_sink->set_pattern(format);
    file_sink   ->set_level  (level);
    file_sink   ->set_pattern(format);
    spdlog::      set_pattern(format);

    sinks_t sinks{console_sink, file_sink};

    spdlog::set_default_logger(std::make_shared<spdlog::logger>(spdlog::logger("KLOG", sinks.begin(), sinks.end())));
    spdlog::set_level(level);
    spdlog::flush_on(spdlog::level::info);

    g_instance = this;

    KLOG("Logger initialized at {} level", LogLevelStrings.at(console_sink->level()));
  }
  catch (const spdlog::spdlog_ex& e)
  {
    std::cout << "Exception caught during logger initialization: " << e.what() << std::endl;
  }
}

klogger::~klogger()
{
  delete g_instance;
}

void klogger::init(const std::string& level)
{
  if (!g_instance)
    g_instance = new klogger(level);
}

static klogger instance()
{
  return *g_instance;
}

} // namespace log
} // namespace kiq

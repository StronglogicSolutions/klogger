#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "time.hpp"
#include "logger.hpp"

namespace kiq::log
{
namespace
{
  static klogger*          g_instance{nullptr};
} // ns
//-------------------------------------------------
using loglevel_names_t = std::map<loglevel,    std::string>;
//-------------------------------------------------
static const loglevel_names_t log_level_names
{
  {loglevel::trace,  "TRACE"  },
  {loglevel::debug,  "DEBUG"  },
  {loglevel::info,   "INFO"   },
  {loglevel::warn,   "WARN"   },
  {loglevel::error,  "ERROR"  },
  {loglevel::fatal,  "FATAL"  },
  {loglevel::silent, "SILENT" }
};
//-------------------------------------------------
colour to_colour(loglevel level)
{
  switch (level)
  {
    case loglevel::warn  : return colour::YELLOW;
    case loglevel::debug : return colour::WHITE;
    case loglevel::fatal : return colour::RED;
    case loglevel::error : return colour::MAGENTA;
    case loglevel::trace : return colour::TURQOISE;
    default:               return colour::GREEN;
  }
}
//-------------------------------------------------
static std::string func_name(const std::source_location& loc)
{
  const std::string full = loc.function_name();
  const auto  opn  = full.find_first_of('(') - 1;
        auto  i    = opn;
  for (; i > 0; i--)
    if (!std::isalpha(full[i]) && full[i] != '_') break;

  return full.substr(i + 1, (opn - i));
}
//-------------------------------------------------
//-------------------------------------------------
  fmt_loc::fmt_loc(const std::string& s, const std::source_location& l)
  : value_(s.c_str()),
    loc_(l) {}
//-------------------------------------------------
  fmt_loc::fmt_loc(const char*        s, const std::source_location& l)
  : value_(s),
    loc_(l) {}
//-------------------------------------------------
//-------------------------------------------------
klogger::klogger(const std::string& name, const std::string& level, const std::string& path)
: path_(path + name + ".kiq_" + std::to_string(getpid()) + ".log")
{
  if (!open_file())
    throw std::runtime_error("Failed to open log file");
}
klogger::~klogger()
{
  delete active_ptr_;
  delete ostream_ptr_;
}
//-------------------------------------------------
void klogger::set_level(loglevel level)
{
  level_ = level;
}
//-------------------------------------------------
loglevel klogger::get_level() const
{
  return level_;
}
//-------------------------------------------------
void klogger::init(const std::string& name, const std::string& level)
{
  if (g_instance)
    return g_instance->log(loglevel::warn, "logger already initialized", std::source_location{});

  g_instance = new klogger(name, level);
  g_instance->set_level(log_level.at(level));
}
//-------------------------------------------------
klogger& klogger::instance()
{
  return *g_instance;
}
//-------------------------------------------------
void klogger::log(loglevel level, const std::string& message, const std::source_location& loc)
{
  const auto full_file = std::string{loc.file_name()};
  const auto file      = full_file.substr(full_file.find_last_of('/') + 1);
  const auto timestamp = localtime_formatted(to_system_time(high_resolution_time_point{std::chrono::system_clock::now()}),
                                             date_formatted + " " + time_formatted);
  active_ptr_->put([this, level, message, loc, timestamp, file]
  {
    std::stringstream ss;
    ss << "\033[" << to_colour(level) << "m"        << timestamp     << " ["          << std::setw(5)
       << log_level_names.at(level)   << "] "       << std::setw(20) << file          << ":"
       << std::setw(3)                << loc.line() << " "           << std::setw(25)
       << func_name(loc)              << "() - "    << message       << "\033[m\n";

    const auto entry = ss.str();
    std::cout << entry;
    buffer_ += entry;

    if (buffer_.size() > flush_limit)
    {
      (*ostream_ptr_) << buffer_ << std::flush;
      buffer_.clear();
    }
  });
}
//-------------------------------------------------
bool klogger::open_file()
{
  auto& outstream = *ostream_ptr_;
  std::ios_base::openmode mode = std::ios_base::out | std::ios_base::trunc;

  outstream.open(path_, mode);

  if (!outstream.is_open())
  {
    std::ostringstream ss_err;
    ss_err << "FILE ERROR:  could not open log file:[" << path_ << "]";
    ss_err << "\n\t\t std::ios_base state = " << outstream.rdstate();

    std::cerr << ss_err.str().c_str() << std::endl;

    outstream.close();

    return false;
  }

  return true;
}

klogger& klog()
{
  return klogger::instance();
}

} // namespace log::kiq

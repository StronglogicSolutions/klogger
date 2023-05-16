#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "logger.hpp"

namespace kiq::log
{
static klogger*          g_instance{nullptr};
//-------------------------------------------------
colour to_colour(const LEVELS level)
{
  switch (level.value)
  {
    case g3::kWarningValue :                                return colour::YELLOW;
    case g3::kDebugValue   :                                return colour::WHITE;
    case g3::kFatalValue   : g3::internal::wasFatal(level); return colour::RED;
    case custom_error      :                                return colour::MAGENTA;
    case custom_trace      :                                return colour::TURQOISE;
    default:                                                return colour::GREEN;
  }
}
//-------------------------------------------------
klogger::klogger(const std::string& name, const std::string& level, const std::string& path)
: path_(path + name + ".kiq" + '_' + std::to_string(getpid()) + ".log")
{
  ostream_ptr_ = new std::ofstream;
  if (!open_file())
    throw std::runtime_error("Failed to open log file");
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
  if (!g_instance)
    g_instance = new klogger(name, level);
  g_instance->set_level(log_level.at(level));
}
//-------------------------------------------------
klogger klogger::instance()
{
  return *g_instance;
}
//-------------------------------------------------
void klogger::to_std_out(const LEVELS& level, const std::string& message, const std::source_location& loc)
{
  const auto full_file = std::string{loc.file_name()};
  const auto file      = full_file.substr(full_file.find_last_of('/') + 1);
  const auto timestamp = g3::localtime_formatted(g3::to_system_time(
    g3::high_resolution_time_point{std::chrono::system_clock::now()}), g3::internal::date_formatted + " " + g3::internal::time_formatted);
  std::stringstream ss;
  ss <<
    "\033[" << to_colour(level) << "m" << timestamp << " [" << level.text << "]\t[" << file << ":" << loc.line() << " " << func_name(loc) << "()]\t" <<
    message << "\033[m\n";
  const auto entry = ss.str();
  std::cout << entry;
  buffer_ += entry;
  if (buffer_.size() > flush_limit)
  {
    (*ostream_ptr_) << buffer_ << std::flush;
    buffer_.clear();
  }
}
//-------------------------------------------------
bool klogger::open_file()
{
  auto& outstream = *ostream_ptr_;
  std::ios_base::openmode mode = std::ios_base::out;
  mode |= std::ios_base::trunc;
  outstream.open(path_, mode);
  if (!outstream.is_open())
  {
    std::ostringstream ss_error;
    ss_error << "FILE ERROR:  could not open log file:[" << path_ << "]";
    ss_error << "\n\t\t std::ios_base state = " << outstream.rdstate();
    std::cerr << ss_error.str().c_str() << std::endl;
    outstream.close();
    return false;
  }
  return true;
}

} // namespace log::kiq

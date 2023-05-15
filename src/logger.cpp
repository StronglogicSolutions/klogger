#include "logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

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
void coloursink::write(g3::LogMessageMover log) const
{
  const auto msg = log.get();
  if (static_cast<int>(klogger::instance().get_level()) < msg._level.value)
    return;

  std::cout       << "\033[" << to_colour(msg._level) << "m" <<
    msg.timestamp() << std::setw(9) << " [" + msg.level()    + "]\t["  +
    msg.file() + ":" + msg.line() + " "     + msg.function() + "()]\t" +
    msg.message() << "\033[m" <<
  std::endl;
}
//-------------------------------------------------
static std::string klog_message_format(const g3::LogMessage& msg)
{
  std::stringstream ss;
  ss << "\033[" << to_colour(msg._level) << "m" <<
    msg.timestamp() << std::setw(9) << " [" + msg.level()    + "]\t["  +
    msg.file() + ":" + msg.line() + " "     + msg.function() + "()]\t" +
    msg.message() << "\033[m\n";
  return ss.str();
}
//-------------------------------------------------
klogger::klogger(const std::string& name, const std::string& level, const std::string& path)
{
  lg_          = g3::LogWorker::createLogWorker();
  auto handler = lg_->addDefaultLogger(name, path, "kiq");
  handler->call   (&g3::FileSink::overrideLogDetails, &klog_message_format);
  lg_    ->addSink(std::make_unique<coloursink>(),    &coloursink::write);
  g3::initializeLogging(lg_.get());
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

} // namespace log::kiq

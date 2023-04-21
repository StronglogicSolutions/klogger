#include "logger.hpp"
#include <iostream>

namespace kiq::log
{

static klogger* g_instance{nullptr};
//-------------------------------------------------
coloursink::colour coloursink::to_colour(const LEVELS level) const
{
  switch (level.value)
  {
    case g3::kWarningValue : return coloursink::colour::YELLOW;
    case g3::kDebugValue   : return coloursink::colour::WHITE;
    case g3::kFatalValue   : g3::internal::wasFatal(level); return coloursink::colour::RED;
    case custom_error      : return coloursink::colour::MAGENTA;
    case custom_trace      : return coloursink::colour::TURQOISE;
    default:                 return coloursink::colour::GREEN;
  }
}
//-------------------------------------------------
void coloursink::write(g3::LogMessageMover log) const
{
  std::cout << "\033[" << to_colour(log.get()._level) << "m" << log.get().toString() << "\033[m" << std::endl;
}
//-------------------------------------------------
klogger::klogger(const std::string& level, const std::string& name, const std::string& path)
{
  lg_ = g3::LogWorker::createLogWorker();
  lg_->addDefaultLogger(name, path);
  lg_->addSink(std::make_unique<coloursink>(), &coloursink::write);
  g3::initializeLogging(lg_.get());
}
//-------------------------------------------------
void klogger::set_level(loglevel level)
{
  level_ = level;
  std::cout << "Set log level to " << static_cast<int>(level) << std::endl;
}
//-------------------------------------------------
void klogger::init(const std::string& level)
{
  if (!g_instance)
    g_instance = new klogger(level);

  g_instance->set_level(log_level.at(level));
}
//-------------------------------------------------
klogger klogger::instance()
{
  return *g_instance;
}

} // namespace log::kiq

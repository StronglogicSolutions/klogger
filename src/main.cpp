#include "logger.hpp"

int main(int argc, char* argv[])
{
  kiq::log::klogger::init("klogger", "trace");
  auto logger = kiq::log::klogger::instance();

  logger.d("Let's test this out with the number %d", std::tuple{99});  // obj logger

  KLOG("Let's test this out with the number %d", 90);      // macro logger
  DLOG("Let's test this out with the number %d", 91);
  TLOG("Let's test this out with the number %d", 92);
  WLOG("Let's test this out with the number %d", 93);
  ELOG("Let's test this out with the number %d", 94);

  std::string org{"Stronglogic Solutions"};

  TLOG("Here is a trace log message from %s", org.c_str());
  // FLOG("We better shut down with the number %d", 95);

  return 0;
}

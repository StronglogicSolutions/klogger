#include "logger.hpp"

int main(int argc, char* argv[])
{
  kiq::log::klogger::init("trace");
  auto logger = kiq::log::klogger::instance();

  logger.d("Let's test this out with the number %d", 99);
  logger.i("Let's test this out with the number %d", 98);
  logger.w("Let's test this out with the number %d", 97);
  logger.t("Let's test this out with the number %d", 96);

  KLOG("Let's test this out with the number %d", 90);
  DLOG("Let's test this out with the number %d", 91);
  TLOG("Let's test this out with the number %d", 92);
  WLOG("Let's test this out with the number %d", 93);
  ELOG("Let's test this out with the number %d", 94);
  // FLOG("We better shut down with the number %d", 95);

  return 0;
}

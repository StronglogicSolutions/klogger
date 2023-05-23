#include "logger.hpp"

int main(int argc, char* argv[])
{
  std::string org{"Stronglogic Solutions"};

  kiq::log::klogger::init("klogger", "trace");
  auto logger = kiq::log::klogger::instance();

  kiq::log::klog()
        .d("Hello {} with string {}",    org, "World");
  logger.w("Hello {} with float   {}",   org, 42.69f);
  logger.i("Hello {} with boolean {}",   org, false);
  logger.t("Hello {} with pointer {:p}", org, static_cast<void*>(&logger));
  logger.e("Hello {} with signed  {}",   org, -64);
  logger.f("Hello {} with integer {}",   org, 99);

  return 0;
}

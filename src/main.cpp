#include "logger.hpp"

int main(int argc, char* argv[])
{
  std::string org{"Stronglogic Solutions"};

  kiq::log::klogger::init("klogger", "trace");
  auto logger = kiq::log::klogger::instance();

  logger.d("Hello {} with integer {}",   org, 99);
  logger.w("Hello {} with float   {}",   org, 42.69f);
  logger.i("Hello {} with boolean {}",   org, false);
  logger.t("Hello {} with pointer {:p}", org, static_cast<void*>(&logger));
  logger.e("Hello {} with signed  {}",   org, -64);

  return 0;
}

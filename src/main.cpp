#include "logger.hpp"

int main(int argc, char* argv[])
{
  kiq::log::klogger::init("trace");
  auto logger = kiq::log::klogger::instance();

  logger.d("Let's test this out with the number %d", 99);
  logger.i("Let's test this out with the number %d", 98);
  logger.w("Let's test this out with the number %d", 97);
  logger.t("Let's test this out with the number %d", 96);
  logger.e("Let's test this out with the number %d", 95);
  logger.d("Let's test this out with the number %d", 99);
  logger.d("Let's test this out with the number %d", 99);
  logger.d("Let's test this out with the number %d", 99);

  // logger.f("Let's test this out with the number %d", 94);

  return 0;
}

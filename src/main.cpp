#include "logger.hpp"

int main(int argc, char* argv[])
{
  kiq::log::klogger::init("klogger", "trace");
  auto logger = kiq::log::klogger::instance();
  std::string org{"Stronglogic Solutions"};
  logger.d("Let's test this out for {} with the number {}", org, 99);  // obj logger
  logger.w("Let's test this out for {} with the number {}", org, 99);
  logger.i("Let's test this out for {} with the number {}", org, 99);
  logger.t("Let's test this out for {} with the number {}", org, 99);
  logger.e("Let's test this out for {} with the number {}", org, 99);

  return 0;
}

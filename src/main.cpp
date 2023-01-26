#include "logger.hpp"

int main(int argc, char* argv[])
{
  kiq::log::klogger::init("debug");

  KLOG("Let's test this out with the number {}", 99);

  return 0;
}
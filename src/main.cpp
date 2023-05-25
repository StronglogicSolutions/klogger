#include "logger.hpp"


namespace outer
{
  namespace inner
  {
    class something
    {
      public:
      static void do_thing()
      {
        something thing;
        thing.hello();
      }

      void hello()
      {
        kiq::log::klog().i("Saying hello from inside");
      }
    };
  }
}

void really_long_function_name() { kiq::log::klog().t("Long function name's log message"); }



int main(int argc, char* argv[])
{
  std::string org{"Stronglogic Solutions"};

  kiq::log::klogger::init("klogger", "trace");
  auto logger = kiq::log::klogger::instance();

  outer::inner::something::do_thing();
  really_long_function_name();
  kiq::log::klog()
        .d("Hello {} with string {}",    org, "World");
  logger.w("Hello {} with float   {}",   org, 42.69f);
  logger.i("Hello {} with boolean {}",   org, false);
  logger.t("Hello {} with pointer {:p}", org, static_cast<void*>(&logger));
  logger.e("Hello {} with signed  {}",   org, -64);
  logger.f("Gdbye {} with integer {}",   org, 99);

  return 0;
}

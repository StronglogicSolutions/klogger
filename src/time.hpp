#include <sys/time.h>
#include <ctime>
#include <string>
#include <cassert>
#include <chrono>

namespace kiq::log
{
using system_time_point          = std::chrono::sys_time<std::chrono::microseconds>;
using high_resolution_time_point =  std::chrono::sys_time<std::chrono::nanoseconds>;
;
static const size_t kFractionalIdentierSize  = 2;
static const std::string kFractionalIdentier = "%f";
static const std::string date_formatted = "%Y/%m/%d";
  // %f: fractions of seconds (%f is nanoseconds)
  // %f3: milliseconds, 3 digits: 001
  // %6: microseconds: 6 digits: 000001  --- default for the time_format
  // %f9, %f: nanoseconds, 9 digits: 000000001
static const std::string time_formatted = "%H:%M:%S %f6";
enum class Fractional {Millisecond, Microsecond, Nanosecond, NanosecondDefault};

inline Fractional getFractional(const std::string& format_buffer, size_t pos)
{
  char  ch  = (format_buffer.size() > pos + kFractionalIdentierSize ? format_buffer.at(pos + kFractionalIdentierSize) : '\0');
  Fractional type = Fractional::NanosecondDefault;
  switch (ch) {
    case '3': type = Fractional::Millisecond; break;
    case '6': type = Fractional::Microsecond; break;
    case '9': type = Fractional::Nanosecond; break;
    default: type = Fractional::NanosecondDefault; break;
  }
  return type;
}

      // Returns the fractional as a string with padded zeroes
      // 1 ms --> 001
      // 1 us --> 000001
      // 1 ns --> 000000001
inline std::string to_string(const system_time_point& ts, Fractional fractional) {
  auto duration = ts.time_since_epoch();
  auto sec_duration = std::chrono::duration_cast<std::chrono::seconds>(duration);
  duration -= sec_duration;
  auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

  auto zeroes = 9; // default ns
  auto digitsToCut = 1; // default ns, divide by 1 makes no change
  switch (fractional) {
    case Fractional::Millisecond : {
        zeroes = 3;
        digitsToCut = 1000000;
        break;
    }
    case Fractional::Microsecond : {
        zeroes = 6;
        digitsToCut = 1000;
        break;
    }
    case Fractional::Nanosecond :
    case Fractional::NanosecondDefault:
    default:
        zeroes = 9;
        digitsToCut = 1;

  }

  ns /= digitsToCut;
  auto value = std::string(std::to_string(ns));
  return std::string(zeroes - value.size(), '0') + value;
}

inline std::string put_time(const struct tm* tmb, const char* c_time_format)
{
  const size_t size = 1024;
  char buffer[size]; // IMPORTANT: check now and then for when gcc will implement std::put_time.
  //                    ... also ... This is way more buffer space then we need

  auto success = std::strftime(buffer, size, c_time_format, tmb);
  // In DEBUG the assert will trigger a process exit. Once inside the if-statement
  // the 'always true' expression will be displayed as reason for the exit
  //
  // In Production mode
  // the assert will do nothing but the format string will instead be returned
  if (0 == success) {
      assert((0 != success) && "strftime fails with illegal formatting");
      return c_time_format;
  }
  return buffer;
  }

inline tm localtime(const std::time_t& ts)
{
  struct tm tm_snapshot;
  localtime_r(&ts, &tm_snapshot); // POSIX
  return tm_snapshot;
}

inline std::string localtime_formatted_fractions(const system_time_point& ts, std::string format_buffer)
{
  // iterating through every "%f" instance in the format string
  auto identifierExtraSize = 0;
  for (size_t pos = 0;
        (pos = format_buffer.find(kFractionalIdentier, pos)) != std::string::npos;
        pos += kFractionalIdentierSize + identifierExtraSize) {
    // figuring out whether this is nano, micro or milli identifier
    auto type = getFractional(format_buffer, pos);
    auto value = to_string(ts, type);
    auto padding = 0;
    if (type != Fractional::NanosecondDefault)
      padding = 1;

    // replacing "%f[3|6|9]" with sec fractional part value
    format_buffer.replace(pos, kFractionalIdentier.size() + padding, value);
  }
  return format_buffer;
}

inline std::string localtime_formatted(const system_time_point& ts, const std::string& time_format)
{
  auto format_buffer = localtime_formatted_fractions(ts, time_format);
  auto time_point = std::chrono::system_clock::to_time_t(ts);
  std::tm t = localtime(time_point);
  return put_time(&t, format_buffer.c_str()); // format example: //"%Y/%m/%d %H:%M:%S");
}
inline system_time_point to_system_time(const high_resolution_time_point& ts)
  {
    // On some (windows) systems, the system_clock does not provide the highest possible time
    // resolution. Thus g3log uses high_resolution_clock for message time stamps. However,
    // unlike system_clock, high_resolution_clock cannot be converted to a time and date as
    // it usually measures reflects the time since power-up.
    // Thus, hrs_now and sys_now are recorded once when the program starts to be able to convert
    // timestamps to dime and date using to_system_time(). The precision of the absolute time is
    // of course that of system_clock() with some error added due to the non-simultaneous initialization
    // of the two static variables but relative times within one log will be as precise as
    // high_resolution_clock.
    using namespace std::chrono;
    // static const auto hrs_now = high_resolution_time_point::clock::now();
    static const auto sys_now = high_resolution_time_point::clock::now();

    // return time_point_cast<system_time_point>(sys_now + (ts - hrs_now));
    return system_time_point::clock::now();
  }
} // ns kiq::log
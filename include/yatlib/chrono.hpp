#pragma once

#include <chrono>

#include "features.hpp"

#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907
#define YAT_INTERNAL_USE_STD_CHRONO
#endif

namespace yat::literals {
inline namespace chrono_literals {

using ::std::literals::chrono_literals::operator"" h;
using ::std::literals::chrono_literals::operator"" min;
using ::std::literals::chrono_literals::operator"" s;
using ::std::literals::chrono_literals::operator"" ms;
using ::std::literals::chrono_literals::operator"" us;
using ::std::literals::chrono_literals::operator"" ns;

}  // namespace chrono_literals
}  // namespace yat::literals

namespace yat::chrono {
using namespace ::std::chrono;
}  // namespace yat::chrono

#ifdef YAT_INTERNAL_USE_STD_CHRONO

namespace yat::chrono {
inline namespace calendar {
// clang-format off
using ::std::chrono::Monday;
using ::std::chrono::Tuesday;
using ::std::chrono::Wednesday;
using ::std::chrono::Thursday;
using ::std::chrono::Friday;
using ::std::chrono::Saturday;
using ::std::chrono::Sunday;

using ::std::chrono::January;
using ::std::chrono::February;
using ::std::chrono::March;
using ::std::chrono::April;
using ::std::chrono::May;
using ::std::chrono::June;
using ::std::chrono::July;
using ::std::chrono::August;
using ::std::chrono::September;
using ::std::chrono::October;
using ::std::chrono::November;
using ::std::chrono::December;
// clang-format on
}  // namespace calendar
}  // namespace yat::chrono

namespace yat::literals {
inline namespace chrono_literals {

using ::std::literals::chrono_literals::operator"" d;
using ::std::literals::chrono_literals::operator"" y;

}  // namespace chrono_literals
}  // namespace yat::literals

#else  // !YAT_INTERNAL_USE_STD_CHRONO

#include <date/date.h>
#include <date/tz.h>

namespace yat::chrono {

//
// Durations
//

using ::date::days;
using ::date::months;
using ::date::weeks;
using ::date::years;

using ::date::clock_time_conversion;

using ::date::is_clock;
using ::date::is_clock_v;

using ::date::sys_days;
using ::date::sys_seconds;
using ::date::sys_time;

using ::date::utc_clock;
using ::date::utc_seconds;
using ::date::utc_time;

using ::date::tai_clock;
using ::date::tai_seconds;

using ::date::gps_clock;
using ::date::gps_seconds;

using ::date::local_days;
using ::date::local_seconds;
using ::date::local_t;
using ::date::local_time;

using ::date::last;
using ::date::last_spec;

using ::date::day;
using ::date::month;
using ::date::year;

using ::date::month_day;
using ::date::month_day_last;
using ::date::month_weekday;
using ::date::month_weekday_last;
using ::date::weekday;
using ::date::weekday_indexed;
using ::date::weekday_last;

using ::date::year_month;
using ::date::year_month_day;
using ::date::year_month_day_last;
using ::date::year_month_weekday;
using ::date::year_month_weekday_last;

using ::date::hh_mm_ss;

using ::date::ambiguous_local_time;
using ::date::choose;
using ::date::local_info;
using ::date::nonexistent_local_time;
using ::date::sys_info;
using ::date::time_zone;
using ::date::time_zone_link;
using ::date::tzdb;
using ::date::tzdb_list;
using ::date::zoned_traits;

using ::date::leap_second;
using ::date::leap_second_info;

using namespace ::date;

inline namespace calendar {
// clang-format off
using ::date::Monday;
using ::date::Tuesday;
using ::date::Wednesday;
using ::date::Thursday;
using ::date::Friday;
using ::date::Saturday;
using ::date::Sunday;

using ::date::January;
using ::date::February;
using ::date::March;
using ::date::April;
using ::date::May;
using ::date::June;
using ::date::July;
using ::date::August;
using ::date::September;
using ::date::October;
using ::date::November;
using ::date::December;
// clang-format on
}  // namespace calendar

}  // namespace yat::chrono

namespace yat::literals {
inline namespace chrono_literals {

#ifdef YAT_IS_GCC_COMPATIBLE
#pragma GCC diagnostic push

// GCC and Clang use different warning flags for the same issue
#if defined(YAT_IS_GCC)
#pragma GCC diagnostic ignored "-Wliteral-suffix"
#elif defined(YAT_IS_CLANG)
#pragma GCC diagnostic ignored "-Wuser-defined-literals"
#endif

#endif

inline constexpr auto operator"" d(unsigned long long d) noexcept {
  return date::literals::operator""_d(d);
}

inline constexpr auto operator"" y(unsigned long long y) noexcept {
  return date::literals::operator""_y(y);
}

#ifdef YAT_IS_GCC_COMPATIBLE
#pragma GCC diagnostic pop
#endif

}  // namespace chrono_literals
}  // namespace yat::literals

#endif  // !YAT_INTERNAL_USE_STD_CHRONO

#undef YAT_INTERNAL_USE_STD_CHRONO
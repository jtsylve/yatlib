#pragma once

#include <chrono>

#if !defined(__cpp_lib_chrono) || __cpp_lib_chrono < 201611L
#error "C++17 std::chrono support is required, but not found"
#elif __cpp_lib_chrono >= 201907
#define YAT_INTERNAL_USE_STD_CHRONO
#endif

namespace yat::chrono {

using namespace std::chrono;

}  // namespace yat::chrono

#ifdef YAT_INTERNAL_USE_STD_CHRONO

namespace yat::chrono {
inline namespace calendar {
// clang-format off
using std::chrono::Monday;
using std::chrono::Tuesday;
using std::chrono::Wednesday;
using std::chrono::Thursday;
using std::chrono::Friday;
using std::chrono::Saturday;
using std::chrono::Sunday;

using std::chrono::January;
using std::chrono::February;
using std::chrono::March;
using std::chrono::April;
using std::chrono::May;
using std::chrono::June;
using std::chrono::July;
using std::chrono::August;
using std::chrono::September;
using std::chrono::October;
using std::chrono::November;
using std::chrono::December;
// clang-format on
}  // namespace calendar
}  // namespace yat::chrono

namespace yat::literals {
inline namespace chrono_literals {
using namespace std::literals::chrono_literals;
}  // namespace chrono_literals
}  // namespace yat::literals

#else  // !YAT_INTERNAL_USE_STD_CHRONO

#include <date/date.h>
#include <date/tz.h>

namespace yat::chrono {
using namespace date;

inline namespace calendar {
// clang-format off
using date::Monday;
using date::Tuesday;
using date::Wednesday;
using date::Thursday;
using date::Friday;
using date::Saturday;
using date::Sunday;

using date::January;
using date::February;
using date::March;
using date::April;
using date::May;
using date::June;
using date::July;
using date::August;
using date::September;
using date::October;
using date::November;
using date::December;
// clang-format on
}  // namespace calendar

}  // namespace yat::chrono

namespace yat::literals {
inline namespace chrono_literals {
using namespace date::literals;
}  // namespace chrono_literals
}  // namespace yat::literals

#endif  // !YAT_INTERNAL_USE_STD_CHRONO

#undef YAT_INTERNAL_USE_STD_CHRONO
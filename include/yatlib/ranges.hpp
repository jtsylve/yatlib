#pragma once

#include "features.hpp"

#if defined(YAT_SUPPORTS_CPP20) && __has_include("ranges")
#include <ranges>
#endif

// Check to see if stdlib support is available
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
#define YAT_INTERNAL_USE_STD_LIB_RANGES
#endif

#ifdef YAT_INTERNAL_USE_STD_LIB_RANGES

namespace yat::ranges {

using namespace ::std::ranges;

}  // namespace yat::ranges

#else  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

namespace yat::ranges {

using namespace ::ranges::cpp20;

}  // namespace yat::ranges

#endif  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#undef YAT_INTERNAL_USE_STD_LIB_RANGES
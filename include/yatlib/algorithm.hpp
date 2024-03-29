#pragma once

#include <algorithm>

// Check to see if stdlib support is available
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 201911L
#define YAT_INTERNAL_USE_STD_LIB_RANGES
#endif

#ifdef YAT_INTERNAL_USE_STD_LIB_RANGES

namespace yat::ranges {

using namespace ::std::ranges;

}  // namespace yat::ranges

#else  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#include <range/v3/algorithm.hpp>

namespace yat::ranges {

using namespace ::ranges::cpp20;

}

#endif  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#undef YAT_INTERNAL_USE_STD_LIB_RANGES
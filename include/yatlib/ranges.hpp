#pragma once

#include "features.hpp"

#if defined(YAT_SUPPORTS_CPP20) && __has_include("ranges")
#include <ranges>
#endif

// Check to see if stdlib support is available
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 201911L
#define YAT_INTERNAL_USE_STD_LIB_RANGES
#endif

#ifdef YAT_INTERNAL_USE_STD_LIB_RANGES

namespace yat::ranges {

using namespace ::std::ranges;

}  // namespace yat::ranges

namespace yat::views {

using namespace ::std::views;

}  // namespace yat::views

#else  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

namespace yat::ranges {

using namespace ::ranges::cpp20;

}  // namespace yat::ranges

namespace yat::views {

using namespace ::ranges::cpp20::views;

}  // namespace yat::views

#endif  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#undef YAT_INTERNAL_USE_STD_LIB_RANGES

// Check to see if stdlib support is available
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
#define YAT_INTERNAL_USE_STD_LIB_RANGES_ZIP
#endif

#ifndef YAT_INTERNAL_USE_STD_LIB_RANGES_ZIP

#include <range/v3/view.hpp>

namespace yat::ranges {

using ::ranges::zip_view;

}  // namespace yat::ranges

namespace yat::views {

using ::ranges::views::zip;

}  // namespace yat::views

#endif  // !YAT_INTERNAL_USE_STD_LIB_RANGES_ZIP

#undef YAT_INTERNAL_USE_STD_LIB_RANGES_ZIP

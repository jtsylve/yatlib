/*
 * Copyright 2020 Joe T. Sylve, Ph.D.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "features.hpp"

#if defined(YAT_SUPPORTS_CPP20) && __has_include("span")
#include <span>
#endif

#include <array>
#include <cassert>
#include <cstddef>
#include <limits>

#include "iterator.hpp"
#include "memory.hpp"
#include "type_traits.hpp"

/////////////////////////////////////////
// P0122R3 - https://wg21.link/P0122R3 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_span) && __cpp_lib_span >= 202002L
#define YAT_INTERNAL_USE_STD_SPAN
#endif

#ifdef YAT_INTERNAL_USE_STD_SPAN

namespace yat {

using std::as_bytes;
using std::as_writable_bytes;
using std::dynamic_extent;
using std::span;

}  // namespace yat

#else

#include <gsl/span>

namespace yat {

using gsl::as_bytes;
using gsl::as_writable_bytes;
using gsl::dynamic_extent;
using gsl::span;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_SPAN

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_SPAN

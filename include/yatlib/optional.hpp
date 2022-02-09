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

// Apple disallows the use of std::optional before macOS 10.14 because the
// bad_optional_access implementation is compiled into libc++.so and is not
// available on those systems.  We provide an inline implementation and attempt
// to disable the macros that prevent the use of std::optional on those systems.

#ifdef YAT_APPLE_CXX17_TYPES_UNAVAILABLE
  #if __has_include("__availability")
  #include <__availability>
  #endif
  #include <__config>

  // Kill the compile error by redefining attribute macros to empty
  #undef _LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
  #undef _LIBCPP_AVAILABILITY_BAD_OPTIONAL_ACCESS

  #define _LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
  #define _LIBCPP_AVAILABILITY_BAD_OPTIONAL_ACCESS

  #ifdef _LIBCPP_OPTIONAL
    #error "This header must be imported prior to any <optional> imports!"
  #endif

  #include <optional>

// Define the bad_optional_access functios that are not in libc++.so on
// older versions of macOS

inline std::bad_optional_access::~bad_optional_access() _NOEXCEPT = default;

inline const char* std::bad_optional_access::what() const _NOEXCEPT {
  return "bad_optional_access";
}
#else
  #include <optional>
#endif  // YAT_APPLE_CXX17_TYPES_UNAVAILABLE

namespace yat {

using std::bad_optional_access;
using std::in_place;
using std::in_place_t;
using std::make_optional;
using std::nullopt;
using std::nullopt_t;
using std::optional;

}  // namespace yat

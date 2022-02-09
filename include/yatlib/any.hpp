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

// Apple disallows the use of std::any before macOS 10.13 because the
// bad_any_cast implementation is compiled into libc++.so and is not
// available on those systems.  We provide an inline implementation and attempt
// to disable the macros that prevent the use of std::any on those systems.

#ifdef YAT_APPLE_CXX17_TYPES_UNAVAILABLE
  #if __has_include("__availability")
  #include <__availability>
  #endif
  #include <__config>

  // Kill the compile error by redefining attribute macros to empty
  #undef _LIBCPP_AVAILABILITY_THROW_BAD_ANY_CAST
  #undef _LIBCPP_AVAILABILITY_BAD_ANY_CAST

  #define _LIBCPP_AVAILABILITY_THROW_BAD_ANY_CAST
  #define _LIBCPP_AVAILABILITY_BAD_ANY_CAST

  #ifdef _LIBCPP_ANY
    #error "This header must be imported prior to any <any> imports!"
  #endif

  #include <any>

// Define the bad_any_cast functios that are not in libc++.so on
// older versions of macOS

inline const char* std::bad_any_cast::what() const _NOEXCEPT {
  return "bad any cast";
}
#else
  #include <any>
#endif  // YAT_APPLE_CXX17_TYPES_UNAVAILABLE

namespace yat {

using std::any;
using std::any_cast;
using std::bad_any_cast;
using std::make_any;

}  // namespace yat

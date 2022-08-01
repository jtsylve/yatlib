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

// Apple disallows the use of std::variant before macOS 10.14 because the
// bad_variant_access::what implementation is compiled into libc++.so and is not
// available on those systems.  We provide an inline implementation and attempt
// to disable the macros that prevent the use of std::variant on those systems.

#ifdef YAT_APPLE_CXX17_TYPES_UNAVAILABLE
#if __has_include("__availability")
#include <__availability>
#endif
#include <__config>

// Kill the compile error by redefining attribute macros to empty
#undef _LIBCPP_AVAILABILITY_THROW_BAD_VARIANT_ACCESS
#undef _LIBCPP_AVAILABILITY_BAD_VARIANT_ACCESS

#define _LIBCPP_AVAILABILITY_THROW_BAD_VARIANT_ACCESS
#define _LIBCPP_AVAILABILITY_BAD_VARIANT_ACCESS

#if defined(_LIBCPP_VARIANT) && !defined(__clang_analyzer__)
#error "This header must be imported prior to any <variant> imports!"
#endif

#include <variant>

// Define the bad_variant_access::what() function that is not in libc++.so on
// older versions of macOS
inline const char* std::bad_variant_access::what() const _NOEXCEPT {
  return "bad_variant_access";
}
#else
#include <variant>
#endif  // YAT_APPLE_CXX17_TYPES_UNAVAILABLE

namespace yat {

using std::bad_variant_access;
using std::get;
using std::get_if;
using std::holds_alternative;
using std::monostate;
using std::variant;
using std::variant_alternative;
using std::variant_alternative_t;
using std::variant_npos;
using std::variant_size;
using std::variant_size_v;
using std::visit;

/////////////////////////////////////////
// P0051R3 - https://wg21.link/P0051R3 //
/////////////////////////////////////////

/// Helper type to allow overloading of lamdas and other function objects.
///
/// This is very useful to use with yat::visit
template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

/// Deduction guide for overloaded struct.
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/////////////////////////////////////////////////
// Match proposal by Nikolai Wuttke            //
//                                             //
// https://www.youtube.com/watch?v=CELWr9roNno //
/////////////////////////////////////////////////

/// Perform variant visiting on overloaded function object handlers
template <typename Variant, typename... Handlers>
auto match(Variant&& v, Handlers&&... handlers) {
  return visit(overloaded{std::forward<Handlers>(handlers)...},
               std::forward<Variant>(v));
}

}  // namespace yat

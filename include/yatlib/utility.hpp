// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cassert>
#include <limits>
#include <type_traits>
#include <utility>

#include "__compat.hpp"
#include "__config.hpp"

#pragma region macros

/// Specifies that an expression will always evaluate to true at a given point.
///
/// On debug builds this is implemented as an assert, on release builds this is
/// implemented as a call to unreachable, which will may cause undefined
/// behavior if the expression is false.
///
/// This is similar to the C++23 attribute [[assume: expr]], with the addition
/// of an assertion on debug builds.
///
/// See: https://en.cppreference.com/w/cpp/language/attributes/assume.
#define YAT_ASSUME(expr) \
  assert(expr);          \
  if (not(expr)) ::yat::unreachable();

#pragma endregion  // macros

namespace yat {
inline namespace v2 {

#pragma region C++20 Library Features
inline namespace YAT_STDCPP20 cpp20 {

#pragma region Integer Comparison Functions
#if YAT_HAS_STDLIB_FEATURE(integer_comparison_functions, 202002L)

using std::cmp_equal;
using std::cmp_greater;
using std::cmp_greater_equal;
using std::cmp_less;
using std::cmp_less_equal;
using std::cmp_not_equal;
using std::in_range;

#else

/// Compare the values of two integers t and u. Unlike builtin comparison
/// operators, negative signed integers always compare less than (and not equal
/// to) unsigned integers: the comparison is safe against lossy integer
/// conversion.
///
/// See: https://en.cppreference.com/w/cpp/utility/intcmp.
template <class T, class U>
constexpr bool cmp_equal(T t, U u) noexcept {
  if constexpr (std::is_signed_v<T> == std::is_signed_v<U>) {
    return t == u;
  } else if constexpr (std::is_signed_v<T>) {
    return t < 0 ? false : std::make_unsigned(t) == u;
  } else {
    return u < 0 ? false : t == std::make_unsigned(u);
  }
}

/// Compare the values of two integers t and u. Unlike builtin comparison
/// operators, negative signed integers always compare less than (and not equal
/// to) unsigned integers: the comparison is safe against lossy integer
/// conversion.
///
/// See: https://en.cppreference.com/w/cpp/utility/intcmp.
template <class T, class U>
constexpr bool cmp_not_equal(T t, U u) noexcept {
  return !cmp_equal(t, u);
}

/// Compare the values of two integers t and u. Unlike builtin comparison
/// operators, negative signed integers always compare less than (and not equal
/// to) unsigned integers: the comparison is safe against lossy integer
/// conversion.
///
/// See: https://en.cppreference.com/w/cpp/utility/intcmp.
template <class T, class U>
constexpr bool cmp_less(T t, U u) noexcept {
  if constexpr (std::is_signed_v<T> == std::is_signed_v<U>) {
    return t < u;
  } else if constexpr (std::is_signed_v<T>) {
    return t < 0 ? true : std::make_unsigned(t) < u;
  } else {
    return u < 0 ? false : t < std::make_unsigned(u);
  }
}

/// Compare the values of two integers t and u. Unlike builtin comparison
/// operators, negative signed integers always compare less than (and not
/// equal to) unsigned integers: the comparison is safe against lossy integer
/// conversion.
///
/// See: https://en.cppreference.com/w/cpp/utility/intcmp.
template <class T, class U>
constexpr bool cmp_greater(T t, U u) noexcept {
  return cmp_less(u, t);
}

/// Compare the values of two integers t and u. Unlike builtin comparison
/// operators, negative signed integers always compare less than (and not
/// equal to) unsigned integers: the comparison is safe against lossy integer
/// conversion.
///
/// See: https://en.cppreference.com/w/cpp/utility/intcmp.
template <class T, class U>
constexpr bool cmp_less_equal(T t, U u) noexcept {
  return !cmp_greater(t, u);
}

/// Compare the values of two integers t and u. Unlike builtin comparison
/// operators, negative signed integers always compare less than (and not
/// equal to) unsigned integers: the comparison is safe against lossy integer
/// conversion.
///
/// See: https://en.cppreference.com/w/cpp/utility/intcmp.
template <class T, class U>
constexpr bool cmp_greater_equal(T t, U u) noexcept {
  return !cmp_less(t, u);
}

/// Returns true if the value of t is in the range of values that can be
/// represented in R, that is, if t can be converted to R without data loss.
///
/// See: https://en.cppreference.com/w/cpp/utility/in_range.
template <class R, class T>
constexpr bool in_range(T t) noexcept {
  return std::cmp_greater_equal(t, std::numeric_limits<R>::min()) &&
         std::cmp_less_equal(t, std::numeric_limits<R>::max());
}

#endif  // YAT_HAS_STDLIB_FEATURE(integer_comparison_functions, 202002L)
#pragma endregion  // Integer Comparison Functions

}  // namespace YAT_STDCPP20 cpp20
#pragma endregion  // C++20 Library Features

#pragma region C++23 Library Features
inline namespace YAT_STDCPP23 cpp23 {

#pragma region std::forward_like
#if YAT_HAS_STDLIB_FEATURE(forward_like, 202207L)

using std::forward_like;

#else

/// Returns a reference to x which has similar properties to T&&.
///
/// See: https://en.cppreference.com/w/cpp/utility/forward_like.
template <class T, class U>
[[nodiscard]] constexpr auto&& forward_like(U&& x) noexcept {
  constexpr bool is_adding_const = std::is_const_v<std::remove_reference_t<T> >;
  if constexpr (std::is_lvalue_reference_v<T&&>) {
    if constexpr (is_adding_const)
      return std::as_const(x);
    else
      return static_cast<U&>(x);
  } else {
    if constexpr (is_adding_const)
      return std::move(std::as_const(x));
    else
      return std::move(x);
  }
}

#endif             // YAT_HAS_STDLIB_FEATURE(forward_like, 202207L)
#pragma endregion  // std::forward_like
#pragma region std::to_underlying
#if YAT_HAS_STDLIB_FEATURE(to_underlying, 202102L)

using std::to_underlying;

#else

/// Converts an enumeration to its underlying type.
///
/// See: https://en.cppreference.com/w/cpp/utility/to_underlying.
template <class Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept {
  return static_cast<std::underlying_type_t<Enum> >(e);
}

#endif             // YAT_HAS_STDLIB_FEATURE(to_underlying, 202102L)
#pragma endregion  // std::to_underlying
#pragma region std::unreachable
#if YAT_HAS_STDLIB_FEATURE(unreachable, 202202L)

using std::unreachable;

#else

/// Invokes undefined behavior. An implementation may use this to optimize
/// impossible code branches away (typically, in optimized builds) or to trap
/// them to prevent further execution (typically, in debug builds).
///
/// See: https://en.cppreference.com/w/cpp/utility/unreachable.
[[noreturn]] inline void unreachable() noexcept {
  // Uses compiler specific extensions if possible, but even if no extension
  // is used, undefined behavior is still raised by an empty function body and
  // the noreturn attribute.
#ifdef YAT_IS_GNU_COMPATIBLE
  __builtin_unreachable();
#elif defined(YAT_IS_MSVC)
  __assume(false);
#endif
}

#endif             // YAT_HAS_STDLIB_FEATURE(unreachable, 202202L)
#pragma endregion  // std::unreachable

}  // namespace YAT_STDCPP23 cpp23
#pragma endregion  // C++23 Library Features

}  // namespace v2
}  // namespace yat

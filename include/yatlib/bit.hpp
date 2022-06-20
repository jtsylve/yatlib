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

#if defined(YAT_SUPPORTS_CPP20) && __has_include("bit")
#include <bit>
#endif

#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

#include "utility.hpp"

#ifdef YAT_IS_MSVC
#include <intrin.h>
#include <stdlib.h>
#endif

/////////////////////////////////////////
// P0463R1 - https://wg21.link/P0463R1 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_endian) && __cpp_lib_endian >= 201907
#define YAT_INTERNAL_USE_STD_ENDIAN
#endif

// Check endianness of compiler.  Clang and gcc support a __BYTE_ORDER__
// definition, and all Windows systems are little endian
#ifdef __BYTE_ORDER__  // Clang and gcc support byteorder
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define YAT_INTERNAL_IS_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define YAT_INTERNAL_IS_BIG_ENDIAN
#else
#error "endian type is not supported"
#endif
#elif defined(_WIN32)  // All Windows systems are all little endian
#define YAT_INTERNAL_IS_LITTLE_ENDIAN
#else
#error "endian detection not supported for this compiler"
#endif

namespace yat {

#ifdef YAT_INTERNAL_USE_STD_ENDIAN
using std::endian;
#else

/// Indicates the endianness of all scalar types
enum class endian {
  little,
  big,
#if defined(YAT_INTERNAL_IS_LITTLE_ENDIAN)
  native = little,
#elif defined(YAT_INTERNAL_IS_BIG_ENDIAN)
  native = big,
#else
  native,
#endif
};

#endif  // YAT_INTERNAL_USE_STD_ENDIAN
}  // namespace yat

////////////////////////////////////////
// P0476R2 - http://wg21.link/P0476R2 //
////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806
#define YAT_INTERNAL_USE_STD_BIT_CAST
#endif

// Check for __builtin_bit_cast
#if YAT_HAS_BUILTIN(__builtin_bit_cast)
#define YAT_INTERNAL_HAS_BUILTIN_BIT_CAST
#endif

// Add feature for constexpr bitcast
#if defined(YAT_INTERNAL_USE_STD_ENDIAN) || \
    defined(YAT_INTERNAL_HAS_BUILTIN_BIT_CAST)
#define YAT_HAS_CONSTEXPR_BIT_CAST
#endif

namespace yat {

#ifdef YAT_INTERNAL_USE_STD_BIT_CAST
using std::bit_cast;
#elif defined(YAT_INTERNAL_HAS_BUILTIN_BIT_CAST)

/// Obtain a value of type To by reinterpreting the object representation of
/// from. Every bit in the value representation of the returned To object is
/// equal to the corresponding bit in the object representation of from. The
/// values of padding bits in the returned To object are unspecified.
///
/// If there is no value of type To corresponding to the value representation
/// produced, the behavior is undefined. If there are multiple such values,
/// which value is produced is unspecified.
template <class To, class From>
[[nodiscard]] YAT_PURE_FUNCTION constexpr auto bit_cast(
    const From& src) noexcept
    -> std::enable_if_t<
        std::conjunction_v<std::bool_constant<sizeof(To) == sizeof(From)>,
                           std::is_trivially_copyable<From>,
                           std::is_trivially_copyable<To>>,
        To> {
  return __builtin_bit_cast(To, src);
}

#else

/// Obtain a value of type To by reinterpreting the object representation of
/// from. Every bit in the value representation of the returned To object is
/// equal to the corresponding bit in the object representation of from. The
/// values of padding bits in the returned To object are unspecified.
///
/// If there is no value of type To corresponding to the value representation
/// produced, the behavior is undefined. If there are multiple such values,
/// which value is produced is unspecified.
///
/// This version of std::bit_cast differs from the official version in two ways.
/// 1.) To is required to be default constructible
/// 2.) This function is not constexpr
template <class To, class From>
[[nodiscard]] YAT_PURE_FUNCTION auto bit_cast(const From& src) noexcept
    -> std::enable_if_t<
        std::conjunction_v<std::bool_constant<sizeof(To) == sizeof(From)>,
                           std::is_trivially_copyable<From>,
                           std::is_trivially_copyable<To>,
                           std::is_default_constructible<To>>,
        To> {
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

#endif  // YAT_INTERNAL_USE_STD_BIT_CAST
}  // namespace yat

/////////////////////////////////////////
// P1272R3 - https://wg21.link/P1272R3 //
/////////////////////////////////////////

// MSVC _byteswap functions aren't constexpr
#ifndef YAT_IS_MSVC
#define YAT_HAS_CONSTEXPR_BYTESWAP
#define YAT_BYTESWAP_CONSTEXPR constexpr
#else
#define YAT_BYTESWAP_CONSTEXPR inline
#endif

namespace yat::detail {

[[nodiscard]] YAT_PURE_FUNCTION YAT_BYTESWAP_CONSTEXPR uint16_t
byteswap(uint16_t value) noexcept {
#ifdef YAT_IS_GCC_COMPATIBLE
  return __builtin_bswap16(value);
#elif defined(YAT_IS_MSVC)
  return _byteswap_ushort(value);
#else
  return static_cast<uint16_t>((value & 0x00FF) << 8) |
         static_cast<uint16_t>((value & 0xFF00) >> 8);
#endif
}

[[nodiscard]] YAT_PURE_FUNCTION YAT_BYTESWAP_CONSTEXPR uint32_t
byteswap(uint32_t value) noexcept {
#ifdef YAT_IS_GCC_COMPATIBLE
  return __builtin_bswap32(value);
#elif defined(YAT_IS_MSVC)
  return _byteswap_ulong(value);
#else
  return ((value & 0x0000'00FFU) << (8 * 3)) |
         ((value & 0x0000'FF00U) << (8 * 1)) |
         ((value & 0x00FF'0000U) >> (8 * 1)) |
         ((value & 0xFF00'0000U) >> (8 * 3));
#endif
}

[[nodiscard]] YAT_PURE_FUNCTION YAT_BYTESWAP_CONSTEXPR uint64_t
byteswap(uint64_t value) noexcept {
#ifdef YAT_IS_GCC_COMPATIBLE
  return __builtin_bswap64(value);
#elif defined(YAT_IS_MSVC)
  return _byteswap_uint64(value);
#else
  return (value & 0x0000'0000'0000'00FFULL) << (8 * 7) |
         (value & 0x0000'0000'0000'FF00ULL) << (8 * 5) |
         (value & 0x0000'0000'00FF'0000ULL) << (8 * 3) |
         (value & 0x0000'0000'FF00'0000ULL) << (8 * 1) |
         (value & 0x0000'00FF'0000'0000ULL) >> (8 * 1) |
         (value & 0x0000'FF00'0000'0000ULL) >> (8 * 3) |
         (value & 0x00FF'0000'0000'0000ULL) >> (8 * 5) |
         (value & 0xFF00'0000'0000'0000ULL) >> (8 * 7);
#endif
}

}  // namespace yat::detail

namespace yat {

/// Takes any integer type and swaps its byteorder to the reverse of its current
/// state.
template <typename IntegerType>
[[nodiscard]] YAT_PURE_FUNCTION YAT_BYTESWAP_CONSTEXPR auto byteswap(
    IntegerType value) noexcept
    -> std::enable_if_t<std::is_integral_v<IntegerType>, IntegerType> {
  if constexpr (sizeof(IntegerType) == 1) {
    return value;
  } else {
    const auto uvalue = static_cast<std::make_unsigned_t<IntegerType>>(value);

    return static_cast<IntegerType>(yat::detail::byteswap(uvalue));
  }
}

}  // namespace yat

////////////////////////////////////////////////////////////////////////////
// PXXXXRX - https://twitter.com/slurpsmadrips/status/1237915884573741057 //
////////////////////////////////////////////////////////////////////////////

// byteswap overload for enums

namespace yat {

/// Takes any enumeration type and swaps its byteorder to the reverse of its
/// current state.
template <typename EnumType>
[[nodiscard]] YAT_PURE_FUNCTION YAT_BYTESWAP_CONSTEXPR auto byteswap(
    EnumType value) noexcept
    -> std::enable_if_t<std::is_enum_v<EnumType>, EnumType> {
  return static_cast<EnumType>(byteswap(to_underlying(value)));
}

}  // namespace yat

/////////////////////////////////////////
// P0553R4 - https://wg21.link/P0553R4 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
#define YAT_INTERNAL_USE_STD_BITOPS
#endif

namespace yat {
#ifdef YAT_INTERNAL_USE_STD_BITOPS

using std::countl_one;
using std::countl_zero;
using std::countr_one;
using std::countr_zero;
using std::popcount;
using std::rotl;
using std::rotr;

#else

/// Computes the result of bitwise left-rotating the value of x by s positions.
/// This operation is also known as a left circular shift
template <typename T>
[[nodiscard]] constexpr auto rotl(T x, int s) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, T> {
  constexpr auto N = std::numeric_limits<T>::digits;
  const auto r = s % N;

  if (r == 0) {
    return x;
  }

  if (r > 0) {
    return static_cast<T>(x << r) | static_cast<T>(x >> (N - r));
  }

  return static_cast<T>(x >> -r) | static_cast<T>(x << (N + r));
}

/// Computes the result of bitwise right-rotating the value of x by s positions.
/// This operation is also known as a right circular shift
template <typename T>
[[nodiscard]] constexpr auto rotr(T x, int s) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, T> {
  constexpr auto N = std::numeric_limits<T>::digits;
  const auto r = s % N;

  if (r == 0) {
    return x;
  }

  if (r > 0) {
    return static_cast<T>(x >> r) | static_cast<T>(x << (N - r));
  }

  return static_cast<T>(x << -r) | static_cast<T>(x >> (N + r));
}

#ifdef YAT_IS_MSVC

/// Returns the number of consecutive 0 bits in the value of x, starting from
/// the most significant bit ("left").
template <typename T>
inline auto countl_zero(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  constexpr int nd = std::numeric_limits<T>::digits;
  static_assert(nd <= std::numeric_limits<unsigned long long>::digits,
                "unsupported bit size");

  if (x == 0) {
    return nd;
  }

  unsigned long result = 0;

  if constexpr (nd <= std::numeric_limits<unsigned>::digits) {
    if (!_BitScanReverse(&result, x)) {
      return nd;
    }
  } else {

#ifdef _M_IX86
    const unsigned int high = x >> 32;
    if (_BitScanReverse(&result, high)) {
      return static_cast<int>(31 - result);
    }

    const auto low = static_cast<unsigned int>(x);
    if (!_BitScanReverse(&result, low)) {
      return nd;
    }
#else   // ^^^ _M_IX86 / !_M_IX86 vvv
    if (!_BitScanReverse64(&result, x)) {
      return nd;
    }
#endif  // _M_IX86
  }

  return nd - 1 - result;
}

/// Returns the number of consecutive 0 bits in the value of x, starting from
/// the least significant bit ("right").
template <typename T>
inline auto countr_zero(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  constexpr int nd = std::numeric_limits<T>::digits;
  static_assert(nd <= std::numeric_limits<unsigned long long>::digits,
                "unsupported bit size");

  if (x == 0) {
    return nd;
  }

  if constexpr (nd < std::numeric_limits<unsigned>::digits) {
    // Cast the smaller type to an unsigned 32-bit but the extended bits need to
    // be set to 1s
    constexpr unsigned mask = std::numeric_limits<unsigned>::max() >> nd << nd;
    return _tzcnt_u32(static_cast<unsigned>(x) | mask);
  }

  else if constexpr (nd == std::numeric_limits<unsigned>::digits) {
    return _tzcnt_u32(x);
  }

  else {
#ifdef _M_IX86
    const auto low = static_cast<unsigned int>(x);

    if (low == 0) {
      const unsigned int high = x >> 32;
      return static_cast<int>(32 + _tzcnt_u32(high));
    }

    return static_cast<int>(_tzcnt_u32(low));
#else   // ^^^ _M_IX86 / !_M_IX86 vvv
    return static_cast<int>(_tzcnt_u64(x));
#endif  // _M_IX86
  }
}

/// Returns the number of 1 bits in the value of x.
template <typename T>
inline auto popcount(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  constexpr int nd = std::numeric_limits<T>::digits;
  static_assert(nd <= std::numeric_limits<unsigned long long>::digits,
                "unsupported bit size");

  if constexpr (nd <= std::numeric_limits<unsigned short>::digits) {
    return __popcnt16(x);
  }

  else if constexpr (nd <= std::numeric_limits<unsigned>::digits) {
    return __popcnt(x);
  }

  else {
#ifdef _M_IX86
    return static_cast<int>(__popcnt(x >> 32) +
                            __popcnt(static_cast<unsigned int>(x)));
#else   // ^^^ _M_IX86 / !_M_IX86 vvv
    return static_cast<int>(__popcnt64(x));
#endif  // _M_IX86
  }
}

#else  // !YAT_IS_MSVC

/// Returns the number of consecutive 0 bits in the value of x, starting from
/// the most significant bit ("left").
template <typename T>
constexpr auto countl_zero(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  constexpr int nd = std::numeric_limits<T>::digits;
  static_assert(nd <= std::numeric_limits<unsigned long long>::digits,
                "unsupported bit size");

  if (x == 0) {
    return nd;
  }

  if constexpr (const int d = std::numeric_limits<unsigned>::digits; nd <= d) {
    constexpr int diff = d - nd;
    return __builtin_clz(x) - diff;
  }

  if constexpr (const int d = std::numeric_limits<unsigned long>::digits;
                nd <= d) {
    constexpr int diff = d - nd;
    return __builtin_clzl(x) - diff;
  }

  if constexpr (const int d = std::numeric_limits<unsigned long long>::digits;
                nd <= d) {
    constexpr int diff = d - nd;
    return __builtin_clzll(x) - diff;
  }
}

/// Returns the number of consecutive 0 bits in the value of x, starting from
/// the least significant bit ("right").
template <typename T>
constexpr auto countr_zero(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  constexpr int nd = std::numeric_limits<T>::digits;
  static_assert(nd <= std::numeric_limits<unsigned long long>::digits,
                "unsupported bit size");

  if (x == 0) {
    return nd;
  }

  if constexpr (nd <= std::numeric_limits<unsigned>::digits) {
    return __builtin_ctz(x);
  }

  if constexpr (nd <= std::numeric_limits<unsigned long>::digits) {
    return __builtin_ctzl(x);
  }

  if constexpr (nd <= std::numeric_limits<unsigned long long>::digits) {
    return __builtin_ctzll(x);
  }
}

/// Returns the number of 1 bits in the value of x.
template <typename T>
constexpr auto popcount(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  constexpr int nd = std::numeric_limits<T>::digits;
  static_assert(nd <= std::numeric_limits<unsigned long long>::digits,
                "unsupported bit size");

  if (x == 0) {
    return 0;
  }

  if constexpr (nd <= std::numeric_limits<unsigned>::digits) {
    return __builtin_popcount(x);
  }

  if constexpr (nd <= std::numeric_limits<unsigned long>::digits) {
    return __builtin_popcountl(x);
  }

  if constexpr (nd <= std::numeric_limits<unsigned long long>::digits) {
    return __builtin_popcountll(x);
  }
}

#endif  // !YAT_IS_MSVC

/// Returns the number of consecutive 1 bits in the value of x, starting from
/// the most significant bit ("left").
template <typename T>
constexpr auto countl_one(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  if (x == std::numeric_limits<T>::max()) {
    return std::numeric_limits<T>::digits;
  }

  return countl_zero(static_cast<T>(~x));
}

/// Returns the number of consecutive 1 bits in the value of x, starting from
/// the least significant bit ("right").
template <typename T>
constexpr auto countr_one(T x) noexcept
    -> std::enable_if_t<std::is_unsigned_v<T>, int> {
  if (x == std::numeric_limits<T>::max()) {
    return std::numeric_limits<T>::digits;
  }

  return countr_zero(static_cast<T>(~x));
}

#endif  // YAT_INTERNAL_USE_STD_BITOPS

}  // namespace yat

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_BITOPS
#undef YAT_INTERNAL_IS_BIG_ENDIAN
#undef YAT_INTERNAL_IS_LITTLE_ENDIAN
#undef YAT_INTERNAL_USE_STD_ENDIAN
#undef YAT_INTERNAL_USE_STD_BIT_CAST
#undef YAT_INTERNAL_HAS_BUILTIN_BIT_CAST

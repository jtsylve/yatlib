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

#if __has_include("bit")
  #include <bit>
#endif

#include <cstring>
#include <type_traits>

#include "features.hpp"

// Checks to see if stdlib support is available

#if defined(__cpp_lib_endian) && __cpp_lib_endian >= 201907
  #define PRIV_YAT_USE_STD_ENDIAN
#endif

#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806
  #define PRIV_YAT_USE_STD_BIT_CAST
#endif

// Check endianness of compiler.  Clang and gcc support a __BYTE_ORDER__
// definition, and all Windows systems are little endian
#ifdef __BYTE_ORDER__  // Clang and gcc support byteorder
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define PRIV_YAT_IS_LITTLE_ENDIAN
  #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define PRIV_YAT_IS_BIG_ENDIAN
  #else
    #error "endian type is not supported"
  #endif
#elif defined(_WIN32)  // All Windows systems are all little endian
  #define PRIV_YAT_IS_LITTLE_ENDIAN
#else
  #error "endian detection not supported for this compiler"
#endif

// Check for __builtin_bit_cast
#if YAT_HAS_BUILTIN(__builtin_bit_cast)
  #define PRIV_YAT_HAS_BUILTIN_BIT_CAST
#endif

// Add feature for constexpr bitcast
#if defined(PRIV_YAT_USE_STD_ENDIAN) || defined(PRIV_YAT_HAS_BUILTIN_BIT_CAST)
  #define YAT_HAS_CONSTEXPR_BIT_CAST
#endif

namespace yat {
#ifdef PRIV_YAT_USE_STD_ENDIAN
using std::endian;
#else

enum class endian {
  little,
  big,
  #if defined(PRIV_YAT_IS_LITTLE_ENDIAN)
  native = little,
  #elif defined(PRIV_YAT_IS_BIG_ENDIAN)
  native = big,
  #else
  native,
  #endif
};

#endif  // PRIV_YAT_USE_STD_ENDIAN

#ifdef PRIV_YAT_USE_STD_BIT_CAST
using std::bit_cast;
#else
  #ifdef PRIV_YAT_HAS_BUILTIN_BIT_CAST

template <
    class To, class From,
    typename = std::enable_if_t<std::conjunction_v<
        std::bool_constant<sizeof(To) == sizeof(From)>,
        std::is_trivially_copyable<From>, std::is_trivially_copyable<To> > > >
YAT_PURE_FUNCTION constexpr To bit_cast(const From &src) noexcept {
  return __builtin_bit_cast(To, src);
}

  #else

/// This version of std::bit_cast differs from the official version in two ways.
/// 1.) To is required to be trivialy default constructible
/// 2.) This function is not constexpr
template <class To, class From,
          typename = std::enable_if_t<std::conjunction_v<
              std::bool_constant<sizeof(To) == sizeof(From)>,
              std::is_trivially_copyable<From>, std::is_trivially_copyable<To>,
              std::is_trivially_default_constructible<To> > > >
YAT_PURE_FUNCTION To bit_cast(const From &src) noexcept {
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

  #endif  // PRIV_YAT_HAS_BUILTIN_BIT_CAST
#endif    // PRIV_YAT_USE_STD_BIT_CAST

}  // namespace yat

#undef PRIV_YAT_IS_BIG_ENDIAN
#undef PRIV_YAT_IS_LITTLE_ENDIAN

#undef PRIV_YAT_USE_STD_ENDIAN
#undef PRIV_YAT_USE_STD_BIT_CAST
#undef PRIV_YAT_HAS_BUILTIN_BIT_CAST

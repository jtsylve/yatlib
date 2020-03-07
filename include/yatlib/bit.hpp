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

#include <cstring>
#include <type_traits>

// Checks to see if stdlib support is available
#if __cplusplus > 201703L && __has_include("bit")
  #include <bit>
  #if defined(__cpp_lib_endian) && __cpp_lib_endian >= 201907
    #define YAT_USE_STD_ENDIAN
  #endif

  #if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806
    #define YAT_USE_STD_BIT_CAST
  #endif
#endif

// Check endianness of compiler.  Clang and gcc support a __BYTE_ORDER__
// definition, and all Windows systems are little endian
#ifdef __BYTE_ORDER__  // Clang and gcc support byteorder
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define YAT_IS_LITTLE_ENDIAN
  #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define YAT_IS_BIG_ENDIAN
  #else
    #error "endian type is not supported"
  #endif
#elif defined(_WIN32)  // All Windows systems are all little endian
  #define YAT_IS_LITTLE_ENDIAN
#else
  #error "endian detection not supported for this compiler"
#endif

namespace yat {
#ifdef YAT_USE_STD_ENDIAN
using std::endian;
#else

enum class endian {
  little,
  big,
  #if defined(YAT_IS_LITTLE_ENDIAN)
  native = little,
  #elif defined(YAT_IS_BIG_ENDIAN)
  native = big,
  #else
  native,
  #endif
};
#endif  // USE_STD_ENDIAN

#ifdef USE_STD_BITCAST
using std::bit_cast;
#else

template <class To, class From,
          typename = std::enable_if_t<(sizeof(To) == sizeof(From)) &&
                                      std::is_trivially_copyable_v<From> &&
                                      std::is_trivial_v<To> > >
constexpr To bit_cast(const From &src) noexcept {
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

#endif  // USE_STD_BITCAST

}  // namespace yat

#undef YAT_IS_BIG_ENDIAN
#undef YAT_IS_LITTLE_ENDIAN

#undef YAT_USE_STD_ENDIAN
#undef YAT_USE_STD_BITCAST

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

#include <cassert>

// Check for consteval support
#if defined(__cpp_consteval) && __cpp_consteval >= 201811
  #define YAT_CONSTEVAL consteval
#else
  #define YAT_CONSTEVAL constexpr
#endif

// Compiler type detection
#if defined(__clang__)
  #define YAT_IS_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
  #define YAT_IS_GCC
#elif _MSC_VER
  #define YAT_IS_MSVC
#else
  #warning "unsupported compiler"
#endif

// Check if compiler has clang/gcc __has_builtin
#ifdef __has_builtin
  #define YAT_HAS_BUILTIN(x) __has_builtin(x)
#else
  #define YAT_HAS_BUILTIN(x) 0
#endif

// Add compiler hints that code should be unreachable
#if defined(YAT_IS_CLANG) || defined(YAT_IS_GCC)
  #define YAT_UNREACHABLE()    \
    do {                       \
      __builtin_unreachable(); \
      assert(false);           \
    } while (0);
#elif defined(YAT_IS_MSVC)
  #define YAT_UNREACHABLE() \
    do {                    \
      __assume(0);          \
      assert(false);        \
    } while (0);
#else
  #define YAT_UNREACHABLE() \
    do {                    \
      assert(false);        \
    } while (0);
#endif

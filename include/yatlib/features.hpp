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

#ifdef YAT_IS_MVSC
#define YAT_CPPVER _MSVC_LANG
#else
#define YAT_CPPVER __cplusplus
#endif

// Check for C++20 support
#if YAT_CPPVER > 201703L
#define YAT_SUPPORTS_CPP20
#endif

#if defined(__cpp_concepts) && __cpp_concepts >= 201907
#define YAT_SUPPORTS_CONCEPTS
#endif

// Check for consteval support
#if defined(__cpp_consteval) && __cpp_consteval >= 201811
#define YAT_CONSTEVAL consteval
#else
#define YAT_CONSTEVAL constexpr
#endif

#if defined(YAT_IS_CLANG) || defined(YAT_IS_GCC)
#define YAT_IS_GCC_COMPATIBLE
#endif

// Check if compiler has clang/gcc __has_builtin
#ifdef __has_builtin
#define YAT_HAS_BUILTIN(x) __has_builtin(x)
#else
#define YAT_HAS_BUILTIN(x) 0
#endif

// Check if compiler has clang/gcc __has_attribute
#if defined(__has_cpp_attribute)
#define YAT_HAS_ATTRIBUTE(x) __has_cpp_attribute(x)
#elif defined(__has_attribute)
#define YAT_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define YAT_HAS_ATTRIBUTE(x) 0
#endif

// Add compiler hints that code should be unreachable
#ifdef YAT_IS_GCC_COMPATIBLE
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

// Add support for ignoring MSVC warnings
#ifdef YAT_IS_MSVC
#define YAT_IGNORE_MSVC_WARNING_PUSH(n) \
  __pragma(warning(push));              \
  __pragma(warning(disable : n));

#define YAT_IGNORE_MSVC_WARNING_POP() __pragma(warning(pop));
#else
#define YAT_IGNORE_MSVC_WARNING_PUSH(n)
#define YAT_IGNORE_MSVC_WARNING_POP()
#endif

// Add pure and const function attribute support
#ifdef YAT_IS_GCC_COMPATIBLE
#define YAT_CONST_FUNCTION [[gnu::const]]
#define YAT_PURE_FUNCTION [[gnu::pure]]
#else
#define YAT_CONST_FUNCTION
#define YAT_PURE_FUNCTION
#endif

// Earlier versions of MSVC have issues with getting decltype of specialized
// functions.  See:
// https://softnfuzzy.blogspot.com/2011/12/decltype-on-visual-studio.html
#ifdef YAT_IS_MSVC
namespace yat::internal {
template <typename T>
T decl_identity(T);
}  // namespace yat::internal
#define YAT_DECLTYPE(T) decltype(yat::internal::decl_identity(T))
#else
#define YAT_DECLTYPE(T) decltype(T)
#endif

// macOS 10.13 and iOS 10.11 don't let you use <any>, <optional>, or <variant>
// even though the headers exist and are publicly noted to work.
// This check is borrowed from abseil.
#if defined(__APPLE__) && defined(_LIBCPP_VERSION) &&             \
    ((defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) &&   \
      __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 101400) ||  \
     (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) &&  \
      __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 120000) || \
     (defined(__ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__) &&   \
      __ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__ < 120000) ||  \
     (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) &&      \
      __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 50000))
#define YAT_APPLE_CXX17_TYPES_UNAVAILABLE
#endif

// macOS 10.15 and iOS 10.13 are required to use the std::filesystem
// even though the headers exist.
#if defined(__APPLE__) && defined(_LIBCPP_VERSION) &&             \
    ((defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) &&   \
      __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 101500) ||  \
     (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) &&  \
      __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 130000) || \
     (defined(__ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__) &&   \
      __ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__ < 130000) ||  \
     (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) &&      \
      __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 60000))
#define YAT_APPLE_CXX17FS_TYPES_UNAVAILABLE
#endif

// Always inline
#if defined(YAT_IS_GCC_COMPATIBLE)
#define YAT_ALWAYS_INLINE [[gnu::always_inline]]
#elif defined(YAT_IS_MSVC)
#define YAT_ALWAYS_INLINE __forceinline
#else
#define YAT_ALWAYS_INLINE
#endif

// no_unique_address
#if YAT_HAS_ATTRIBUTE(no_unique_address)
#define YAT_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif YAT_HAS_ATTRIBUTE(msvc::no_unique_address)
#define YAT_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define YAT_NO_UNIQUE_ADDRESS
#endif

// Used
#if defined(YAT_IS_GCC_COMPATIBLE)
#define YAT_USED [[gnu::used]]
#else
#define YAT_USED
#endif

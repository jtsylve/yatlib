// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

/// Macro to detect if the given standard library feature is available.
///
/// This is used internally in order to provide fallback implementations of
/// unsupported features.
///
/// If YAT_NO_STDLIB_FALLBACKS is defined,then this macro will always return
/// false and the fallback implementation will be used regardless of standard
/// library support.  This is useful for testing purposes.
#define YAT_HAS_STDLIB_FEATURE(feature, version)                           \
  (not defined(YAT_NO_STDLIB_FALLBACKS) && defined(__cpp_lib_##feature) && \
   __cpp_lib_##feature >= version)

//
// Standard C++ Compatibility Macros
//
// These macros are used to denote compatibility with versions of the
// C++ standard, and can be used to identify and/or deprecate code that
// is no longer needed.
//
#define YAT_STDCPP26
#define YAT_STDCPP23
#define YAT_STDCPP20
#define YAT_STDCPP17 [[deprecated("use the std namespace for C++17 features")]]
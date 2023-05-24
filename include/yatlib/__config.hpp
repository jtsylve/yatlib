// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

//
// Compiler Detection
//

#if defined(__clang__)
#define YAT_IS_CLANG
#elif defined(__INTEL_COMPILER)
#define YAT_IS_INTEL
#elif defined(__GNUC__)
#define YAT_IS_GCC
// Detecting MSVC
#elif defined(_MSC_VER)
#define YAT_IS_MSVC
#endif

#ifdef __GNUC__
#define YAT_IS_GNU_COMPATIBLE
#endif

//
// C++ Standard Detection
//

#ifdef YAT_IS_MSVC
#define YAT_CPP_STANDARD _MSVC_LANG
#else
#define YAT_CPP_STANDARD __cplusplus
#endif

//
// Feature Detection
//

#if __cpp_concepts < 201907L
#error "yatlib requires C++20 concepts support"
#endif

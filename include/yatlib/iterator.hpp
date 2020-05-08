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

#include <iterator>

#include "type_traits.hpp"

/////////////////////////////////////////
// P1464R1 - https://wg21.link/P1464R1 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 201911L
  #define YAT_INTERNAL_USE_STD_ITER_REFERENCE_T
#endif

#ifdef YAT_INTERNAL_USE_STD_ITER_REFERENCE_T

namespace yat {
using std::iter_reference_t;
}  // namespace yat

#else

namespace yat {

/// Computes the reference type of T.
template <typename T, typename = std::enable_if_t<is_dereferencable_v<T> > >
using iter_reference_t = decltype(*std::declval<T&>());

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_ITER_REFERENCE_T

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_ITER_REFERENCE_T

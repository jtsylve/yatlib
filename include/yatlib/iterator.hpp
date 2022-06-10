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
using std::incrementable_traits;
using std::iter_reference_t;
}  // namespace yat

#else

namespace yat::detail {

// Since we don't have access to concepts, we have to get clever with templates
// to handle the incremental_traits specializations

// Enables pointer type specialization, but only for object pointers
template <typename T, typename = std::enable_if_t<std::is_object_v<T> > >
using object_ptr_t = T *;

// Tells if an object has a predefined difference_type
template <typename, typename = void>
inline constexpr bool has_difference_type_v = false;

template <typename T>
inline constexpr bool
    has_difference_type_v<T, std::void_t<typename T::difference_type> > = true;

// Tells if we can infer a difference type based on the difference operator
template <typename T, typename = void>
inline constexpr bool has_difference_operator_v = false;

template <typename T>
inline constexpr bool has_difference_operator_v<
    T, std::void_t<decltype(std::declval<T const &>() -
                            std::declval<T const &>())> > = true;

// Defines difference_result type based off of difference operator
template <typename T,
          typename = std::enable_if_t<has_difference_operator_v<T> > >
using difference_result_t =
    std::make_signed_t<decltype(std::declval<T const &>() -
                                std::declval<T const &>())>;

// V1 types are types that have a difference_type defined
template <typename, typename = void>
struct incrementable_traits_v1 {};

template <typename T>
struct incrementable_traits_v1<T, std::void_t<typename T::difference_type> > {
  using difference_type = typename T::difference_type;
};

// V2 types do not have difference_types defined
template <typename, typename = void>
struct incrementable_traits_v2 {};

template <typename T>
struct incrementable_traits_v2<T, std::void_t<difference_result_t<T> > > {
  using difference_type = difference_result_t<T>;
};

}  // namespace yat::detail

namespace yat {

/// Computes the associated difference type of the type, if any. Users may
/// specialize incrementable_traits for a program-defined type.
template <typename T>
struct incrementable_traits
    : std::conditional_t<detail::has_difference_type_v<T>,
                         detail::incrementable_traits_v1<T>,
                         detail::incrementable_traits_v2<T> > {};

template <typename T>
struct incrementable_traits<detail::object_ptr_t<T> > {
  using difference_type = std::ptrdiff_t;
};

template <typename T>
struct incrementable_traits<const T> : incrementable_traits<T> {};

/// Computes the reference type of T.
template <typename T, typename = std::enable_if_t<is_dereferencable_v<T> > >
using iter_reference_t = decltype(*std::declval<T &>());

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_ITER_REFERENCE_T

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_ITER_REFERENCE_T

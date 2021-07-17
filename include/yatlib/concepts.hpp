/*
 * Copyright 2021 Joe T. Sylve, Ph.D.
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

#ifdef YAT_SUPPORTS_CONCEPTS

#include "type_traits.hpp"

#if __has_include("concepts")
#include <concepts>
#endif

#if defined(__cpp_lib_concepts) && __cpp_lib_concepts >= 202002
#define YAT_INTERNAL_USE_STD_LIB_CONCEPTS
#endif

//
// Standard Library Concepts
//

#ifdef YAT_INTERNAL_USE_STD_LIB_CONCEPTS

namespace yat {

using std::constructible_from;
using std::convertible_to;
using std::copy_constructible;
using std::default_initializable;
using std::derived_from;
using std::destructible;
using std::floating_point;
using std::integral;
using std::move_constructible;
using std::same_as;
using std::signed_integral;
using std::unsigned_integral;

}  // namespace yat

#else  // YAT_INTERNAL_USE_STD_LIB_CONCEPTS

namespace yat {

/// The concept same_as<T, U> is satisfied if and only if T and U denote the
/// same type.
///
/// std::same_as<T, U> subsumes std::same_as<U, T> and vice versa.
template <typename T, typename U>
concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;

/// The concept derived_from<Derived, Base> is satisfied if and only if Base is
/// a class type that is either Derived or a public and unambiguous base of
/// Derived, ignoring cv-qualifiers.
///
/// Note that this behaviour is different to std::is_base_of when Base is a
/// private or protected base of Derived.
template <typename Derived, typename Base>
concept derived_from = std::is_base_of_v<Base, Derived> &&
    std::is_convertible_v<const volatile Derived*, const volatile Base*>;

/// The concept convertible_to<From, To> specifies that an expression of the
/// same type and value category as those of std::declval<From>() can be
/// implicitly and explicitly converted to the type To, and the two forms of
/// conversion are equivalent.
template <typename From, typename To>
concept convertible_to = std::is_convertible_v<From, To> &&
    requires(std::add_rvalue_reference_t<From> (&f)()) {
  static_cast<To>(f());
};

/// The concept integral<T> is satisfied if and only if T is an integral type.
template <typename T>
concept integral = std::is_integral_v<T>;

/// The concept signed_integral<T> is satisfied if and only if T is an integral
/// type and std::is_signed_v<T> is true.
template <typename T>
concept signed_integral = integral<T> && std::is_signed_v<T>;

/// The concept unsigned_integral<T> is satisfied if and only if T is an
/// integral type and std::is_signed_v<T> is false.
template <typename T>
concept unsigned_integral = integral<T> && !signed_integral<T>;

/// The concept floating_point<T> is satisfied if and only if T is a
/// floating-point type.
template <typename T>
concept floating_point = std::is_floating_point_v<T>;

/// The concept destructible specifies the concept of all types whose instances
/// can safely be destroyed at the end of their lifetime (including reference
/// types).
template <typename T>
concept destructible = std::is_nothrow_destructible_v<T>;

/// The constructible_from concept specifies that a variable of type T can be
/// initialized with the given set of argument types Args....
template <typename T, typename... Args>
concept constructible_from =
    destructible<T> && std::is_constructible_v<T, Args...>;

/// The default_initializable concept checks whether variables of type T can be
///
/// * value-initialized (T() is well-formed);
/// * direct-list-initialized from an empty initializer list (T{} is
/// well-formed); and
/// * default-initialized (T t; is well-formed).
///
/// Access checking is performed as if in a context unrelated to T. Only the
/// validity of the immediate context of the variable initialization is
/// considered.
template <typename T>
concept default_initializable = constructible_from<T> && requires {
  T{};
  ::new (static_cast<void*>(nullptr)) T;
};

/// The concept move_constructible is satisfied if T is a reference type, or if
/// it is an object type where an object of that type can be constructed from an
/// rvalue of that type in both direct- and copy-initialization contexts, with
/// the usual semantics.
template <typename T>
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

/// The concept copy_constructible is satisfied if T is an lvalue reference
/// type, or if it is a move_constructible object type where an object of that
/// type can constructed from a (possibly const) lvalue or const rvalue of that
/// type in both direct- and copy-initialization contexts with the usual
/// semantics (a copy is constructed with the source unchanged).
template <typename T>
concept copy_constructible = move_constructible<T> &&
    constructible_from<T, T&> && convertible_to<T&, T> &&
    constructible_from<T, const T&> && convertible_to<const T&, T> &&
    constructible_from<T, const T> && convertible_to<const T, T>;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_LIB_CONCEPTS

//
// Custom Concepts
//

namespace yat {

/// Specifies that a given type is an alternative type of a given variant type
template <typename T, typename VariantType>
concept variant_alternative_type = is_variant_alternative_v<T, VariantType>;

/// Specifies that a givent type is trivially copyable
template <typename T>
concept trivially_copyable = std::is_trivially_copyable_v<T>;

}  // namespace yat

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_LIB_CONCEPTS

#endif  // YAT_SUPPORTS_CONCEPTS
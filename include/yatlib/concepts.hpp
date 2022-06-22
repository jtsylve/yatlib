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

#if defined(YAT_SUPPORTS_CPP20) && __has_include("concepts")
#include <concepts>
#endif

/////////////////////////////////////////
// P0898R3 - https://wg21.link/P0898R3 //
/////////////////////////////////////////

#if defined(__cpp_lib_concepts) && __cpp_lib_concepts >= 202002
#define YAT_INTERNAL_USE_STD_LIB_CONCEPTS
#endif

//
// Standard Library Concepts
//

#ifdef YAT_INTERNAL_USE_STD_LIB_CONCEPTS

namespace yat {

//
// core language concepts
//
using std::assignable_from;
using std::common_reference_with;
using std::common_with;
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
using std::swappable;
using std::swappable_with;
using std::unsigned_integral;

//
// comparison concepts
//
using std::equality_comparable;
using std::equality_comparable_with;
using std::totally_ordered;
using std::totally_ordered_with;

//
// Object concepts
//
using std::copyable;
using std::movable;
using std::regular;
using std::semiregular;

//
// Callable concepts
//
using std::equivalence_relation;
using std::invocable;
using std::predicate;
using std::regular_invocable;
using std::relation;
using std::strict_weak_order;

}  // namespace yat

#else  // YAT_INTERNAL_USE_STD_LIB_CONCEPTS

#include <concepts/concepts.hpp>
#include <range/v3/functional/concepts.hpp>

namespace yat {

//
// core language concepts
//
using ::concepts::assignable_from;
using ::concepts::common_reference_with;
using ::concepts::common_with;
using ::concepts::constructible_from;
using ::concepts::convertible_to;
using ::concepts::copy_constructible;
using ::concepts::derived_from;
using ::concepts::destructible;
using ::concepts::integral;
using ::concepts::move_constructible;
using ::concepts::same_as;
using ::concepts::signed_integral;
using ::concepts::swappable;
using ::concepts::swappable_with;
using ::concepts::unsigned_integral;

//
// comparison concepts
//
using ::concepts::equality_comparable;
using ::concepts::equality_comparable_with;
using ::concepts::totally_ordered;
using ::concepts::totally_ordered_with;

//
// Object concepts
//
using ::concepts::copyable;
using ::concepts::movable;
using ::concepts::regular;
using ::concepts::semiregular;

//
// Callable concepts
//
using ::ranges::cpp20::invocable;
using ::ranges::cpp20::predicate;
using ::ranges::cpp20::regular_invocable;
using ::ranges::cpp20::relation;
using ::ranges::cpp20::strict_weak_order;

//
// Missing concepts from the ranges implementation
//

/// The concept floating_point<T> is satisfied if and only if T is a
/// floating-point type.
template <typename T>
concept floating_point = std::is_floating_point_v<T>;

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

/// The concept equivalence_relation<R, T, U> specifies that the relation R
/// imposes an equivalence relation on its arguments.
template <typename R, typename T, typename U>
concept equivalence_relation = relation<R, T, U>;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_LIB_CONCEPTS

//
// Custom Concepts
//

namespace yat {

/// Specifies that a given type is an alternative type of a given variant type
template <typename T, typename VariantType>
concept variant_alternative_type = is_variant_alternative_v<T, VariantType>;

/// Specifies that a given type is trivially copyable
template <typename T>
concept trivially_copyable = std::is_trivially_copyable_v<T>;

/// Specifies that a given type is a char type
template <typename T>
concept char_type = is_char_type_v<T>;

/// Specifies that a given type can be indexed using the subscript operator
template <typename T>
concept indexable = requires(T& t, size_t i) {
  t[i];
};

/// Specificies that a given type is a specialization of a templated type
template <typename T, template <typename...> typename TT>
concept specializes = is_specialization_of_v<T, TT>;

}  // namespace yat

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_LIB_CONCEPTS

#endif  // YAT_SUPPORTS_CONCEPTS
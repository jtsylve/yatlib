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

#include "features.hpp"
#include "type_traits.hpp"

/////////////////////////////////////////
// P1464R1 - https://wg21.link/P1464R1 //
/////////////////////////////////////////

#if defined(YAT_SUPPORTS_CPP20) && __has_include("ranges")
#include <ranges>
#endif

// Check to see if stdlib support is available
#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 201911L
#define YAT_INTERNAL_USE_STD_LIB_RANGES
#endif

#ifdef YAT_INTERNAL_USE_STD_LIB_RANGES

namespace yat {

using std::indirect_result_t;
using std::projected;

using std::iter_common_reference_t;
using std::iter_difference_t;
using std::iter_reference_t;
using std::iter_rvalue_reference_t;
using std::iter_value_t;

using std::common_iterator;
using std::counted_iterator;
using std::default_sentinel;
using std::default_sentinel_t;
using std::move_sentinel;
using std::unreachable_sentinel;
using std::unreachable_sentinel_t;

// Specialize these in the ::std namespace
#define YAT_ITER_SPEC_NAMESPACE std

using std::incrementable_traits;
using std::indirectly_readable_traits;

#ifdef YAT_SUPPORTS_CONCEPTS

using std::bidirectional_iterator;
using std::contiguous_iterator;
using std::forward_iterator;
using std::incrementable;
using std::indirect_result_t;
using std::indirect_strict_weak_order;
using std::indirect_unary_predicate;
using std::indirectly_comparable;
using std::indirectly_copyable;
using std::indirectly_copyable_storable;
using std::indirectly_movable;
using std::indirectly_movable_storable;
using std::indirectly_readable;
using std::indirectly_regular_unary_invocable;
using std::indirectly_swappable;
using std::indirectly_unary_invocable;
using std::indirectly_writable;
using std::input_iterator;
using std::input_or_output_iterator;
using std::mergeable;
using std::output_iterator;
using std::permutable;
using std::projected;
using std::random_access_iterator;
using std::sentinel_for;
using std::sized_sentinel_for;
using std::sortable;
using std::weakly_incrementable;

#endif

}  // namespace yat

#else  // !YAT_INTERNAL_USE_STD_LIB_RANGES

#include <range/v3/iterator.hpp>

namespace yat {

using ::ranges::cpp20::indirect_result_t;
using ::ranges::cpp20::projected;

using ::ranges::cpp20::iter_common_reference_t;
using ::ranges::cpp20::iter_difference_t;
using ::ranges::cpp20::iter_reference_t;
using ::ranges::cpp20::iter_rvalue_reference_t;
using ::ranges::cpp20::iter_value_t;

using ::ranges::cpp20::common_iterator;
using ::ranges::cpp20::counted_iterator;
using ::ranges::cpp20::default_sentinel;
using ::ranges::cpp20::default_sentinel_t;
using ::ranges::cpp20::move_sentinel;
inline constexpr auto unreachable_sentinel = ::ranges::cpp20::unreachable;
using ::ranges::cpp20::unreachable_sentinel_t;

// Specialize these in the ::ranges namespace
#define YAT_ITER_SPEC_NAMESPACE ranges

using ::ranges::cpp20::incrementable_traits;
using ::ranges::cpp20::indirectly_readable_traits;

#ifdef YAT_SUPPORTS_CONCEPTS

using ::ranges::cpp20::bidirectional_iterator;
using ::ranges::cpp20::contiguous_iterator;
using ::ranges::cpp20::forward_iterator;
using ::ranges::cpp20::incrementable;
using ::ranges::cpp20::indirect_result_t;
using ::ranges::cpp20::indirect_strict_weak_order;
using ::ranges::cpp20::indirect_unary_predicate;
using ::ranges::cpp20::indirectly_comparable;
using ::ranges::cpp20::indirectly_copyable;
using ::ranges::cpp20::indirectly_copyable_storable;
using ::ranges::cpp20::indirectly_movable;
using ::ranges::cpp20::indirectly_movable_storable;
using ::ranges::cpp20::indirectly_readable;
using ::ranges::cpp20::indirectly_regular_unary_invocable;
using ::ranges::cpp20::indirectly_swappable;
using ::ranges::cpp20::indirectly_unary_invocable;
using ::ranges::cpp20::indirectly_writable;
using ::ranges::cpp20::input_iterator;
using ::ranges::cpp20::input_or_output_iterator;
using ::ranges::cpp20::mergeable;
using ::ranges::cpp20::output_iterator;
using ::ranges::cpp20::permutable;
using ::ranges::cpp20::projected;
using ::ranges::cpp20::random_access_iterator;
using ::ranges::cpp20::sentinel_for;
using ::ranges::cpp20::sized_sentinel_for;
using ::ranges::cpp20::sortable;
using ::ranges::cpp20::weakly_incrementable;

#endif

}  // namespace yat

#endif  // !YAT_INTERNAL_USE_STD_LIB_RANGES

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_LIB_RANGES

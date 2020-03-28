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

#include <type_traits>

#include "variant.hpp"

/////////////////////////////////////////////////////////////
// Determine if a given type is a member of a std::variant //
/////////////////////////////////////////////////////////////

namespace yat {

/// Determines if a given type is a member type of a variant.
template <typename T, typename VariantType>
struct is_variant_member;

/// Determines if a given type is a member type of a variant.
template <typename T, typename... Types>
struct is_variant_member<T, yat::variant<Types...>>
    : public std::disjunction<std::is_same<T, Types>...> {};

/// Determines if a given type is a member type of a variant.
template <typename T, typename VariantType>
using is_variant_member_t = typename is_variant_member<T, VariantType>::type;

/// Determines if a given type is a member type of a variant.
template <typename T, typename VariantType>
constexpr bool is_variant_member_v = is_variant_member<T, VariantType>::value;

}  // namespace yat

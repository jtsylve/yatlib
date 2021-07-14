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

#ifdef YAT_SUPPORTS_CPP20

  #include "type_traits.hpp"

namespace yat {

/// Specifies that a given type is an alternative type of a given variant type
template <typename T, typename VariantType>
concept variant_alternative_type = is_variant_alternative_v<T, VariantType>;

}  // namespace yat

#endif  // YAT_SUPPORTS_CPP20
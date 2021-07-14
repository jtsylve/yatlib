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
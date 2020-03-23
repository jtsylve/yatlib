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

#include <cstdint>
#include <cstdlib>

#include "bit.hpp"
#include "features.hpp"

namespace yat {

/// Tells whether we're compiling for a little endian system
constexpr bool is_little_endian_system = (endian::native == endian::little);

/// Tells whether we're compiling for a big endian system
constexpr bool is_big_endian_system = (endian::native == endian::big);

/// Type support for handling scalar values with regards to endianess
template <typename T, endian Endianess,
          typename = std::enable_if_t<
              std::is_nothrow_invocable_r_v<T, decltype(byteswap<T>), T>>>
class base_endian_scalar {
 public:
  using value_type = T;

  constexpr base_endian_scalar() noexcept = default;

  // cppcheck-suppress noExplicitConstructor
  base_endian_scalar(const T& value) noexcept : _value{to_native(value)} {}

  inline operator T() const noexcept { return to_native(_value); }

  [[nodiscard]] inline T value() const noexcept { return to_native(_value); }

  // We can get a performance improvement by allowing direct equality
  // comparisions of types with the same Endianess

  template <typename U>
  friend constexpr bool operator==(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, Endianess>& rhs) noexcept {
    return (lhs._value == rhs._value);
  }

  template <typename U>
  friend constexpr bool operator!=(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, Endianess>& rhs) noexcept {
    return (lhs._value != rhs._value);
  }

 private:
  [[nodiscard]] YAT_PURE_FUNCTION static inline T to_native(
      const T& value) noexcept {
    if constexpr (Endianess == endian::native) {
      return value;
    } else {
      return byteswap(value);
    }
  }

  T _value{};

  template <typename, endian, typename>
  friend class base_endian_scalar;
};

template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
using big_scaler = base_endian_scalar<T, endian::big>;

template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
using little_scaler = base_endian_scalar<T, endian::little>;

using big_int8_t = big_scaler<int8_t>;
using big_uint8_t = big_scaler<uint8_t>;
using big_int16_t = big_scaler<int16_t>;
using big_uint16_t = big_scaler<uint16_t>;
using big_int32_t = big_scaler<int32_t>;
using big_uint32_t = big_scaler<uint32_t>;
using big_int64_t = big_scaler<int64_t>;
using big_uint64_t = big_scaler<uint64_t>;
using big_intptr_t = big_scaler<intptr_t>;
using big_uintptr_t = big_scaler<uintptr_t>;

using little_int8_t = little_scaler<int8_t>;
using little_uint8_t = little_scaler<uint8_t>;
using little_int16_t = little_scaler<int16_t>;
using little_uint16_t = little_scaler<uint16_t>;
using little_int32_t = little_scaler<int32_t>;
using little_uint32_t = little_scaler<uint32_t>;
using little_int64_t = little_scaler<int64_t>;
using little_uint64_t = little_scaler<uint64_t>;
using little_intptr_t = little_scaler<intptr_t>;
using little_uintptr_t = little_scaler<uintptr_t>;

}  // namespace yat

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

#include <absl/base/internal/endian.h>

#include <cstdint>

#include "bit.hpp"
#include "features.hpp"

namespace yat {

/// Tells whether we're compiling for a little endian system
constexpr bool is_little_endian_system = (endian::native == endian::little);

/// Tells whether we're compiling for a big endian system
constexpr bool is_big_endian_system = (endian::native == endian::big);

/// Creates a new scalar value with the opposite endianness of the one provided
template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
inline T swap_endian(const T& value) noexcept {
  if constexpr (sizeof(T) == 1) {
    return value;
  }

  if constexpr (sizeof(T) == sizeof(uint16_t)) {
    return bit_cast<T>(absl::gbswap_16(bit_cast<uint16_t>(value)));
  }

  if constexpr (sizeof(T) == sizeof(uint32_t)) {
    return bit_cast<T>(absl::gbswap_32(bit_cast<uint32_t>(value)));
  }

  if constexpr (sizeof(T) == sizeof(uint64_t)) {
    return bit_cast<T>(absl::gbswap_64(bit_cast<uint64_t>(value)));
  }

  YAT_UNREACHABLE();
}

/// Type support for handling scalar values with regards to endianess
template <typename T, endian endianess,
          typename = std::enable_if_t<std::is_scalar_v<T>>>
class base_endian_scalar {
 public:
  constexpr base_endian_scalar() noexcept = default;

  // cppcheck-suppress noExplicitConstructor
  base_endian_scalar(const T& value) noexcept : _value{to_native(value)} {}

  inline operator T() const noexcept { return to_native(_value); }

  // We can get a performance improvement by allowing direct comparisions of
  // scalar types with the same endianess

  template <typename U>
  friend constexpr bool operator==(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, endianess>& rhs) noexcept {
    return (lhs._value == rhs._value);
  }

  template <typename U>
  friend constexpr bool operator!=(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, endianess>& rhs) noexcept {
    return (lhs._value != rhs._value);
  }

  template <typename U>
  friend constexpr bool operator<(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, endianess>& rhs) noexcept {
    return (lhs._value < rhs._value);
  }

  template <typename U>
  friend constexpr bool operator>(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, endianess>& rhs) noexcept {
    return (lhs._value > rhs._value);
  }

  template <typename U>
  friend constexpr bool operator<=(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, endianess>& rhs) noexcept {
    return (lhs._value <= rhs._value);
  }

  template <typename U>
  friend constexpr bool operator>=(
      const base_endian_scalar& lhs,
      const base_endian_scalar<U, endianess>& rhs) noexcept {
    return (lhs._value >= rhs._value);
  }

 private:
  static inline T to_native(const T& value) noexcept {
    if constexpr (endianess == endian::native) {
      return value;
    }

    return swap_endian(value);
  }

  T _value{};

  template <typename, endian, typename>
  friend class base_endian_scalar;
};

template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
using be = base_endian_scalar<T, endian::big>;

template <typename T, typename = std::enable_if_t<std::is_scalar_v<T>>>
using le = base_endian_scalar<T, endian::little>;

}  // namespace yat

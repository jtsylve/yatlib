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

#include <limits>
#include <vector>

#include "endian.hpp"

// Test for constexpr vectors
#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L
#define YAT_BITMAP_CONSTEXPR constexpr
#else
#define YAT_BITMAP_CONSTEXPR
#endif

namespace yat {

/// `bitmap` represents a sequence of bits that can be manipulated efficiently.
///
/// This is similar to std::vector<bool>, but the layout of the bits in memory
/// is well defined.
class bitmap {
  using storage_type = little_uint64_t;
  static constexpr size_t storage_bits =
      std::numeric_limits<storage_type::value_type>::digits;

  /// Calculate the number of storage ints we need to store n bits
  static constexpr size_t storage_size(size_t n) noexcept {
    return n / storage_bits + ((n % storage_bits) != 0);
  }

  /// Calculate the storage index
  static constexpr size_t si(size_t n) noexcept { return n / storage_bits; }

  /// Calculate the bit index
  static constexpr size_t bi(size_t n) noexcept { return n % storage_bits; }

  /// Calculate the bitmask for an index
  static constexpr storage_type bm(size_t n) noexcept { return 1ULL << bi(n); }

 public:
  /// Create an empty bitmap
  YAT_BITMAP_CONSTEXPR bitmap() noexcept = default;

  /// Create a bitmap with `n` unset bits
  explicit YAT_BITMAP_CONSTEXPR bitmap(size_t n)
      : _storage{storage_size(n)}, _count{n} {}

  /// Access a given bit.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  YAT_BITMAP_CONSTEXPR bool operator[](size_t n) const {
    return (_storage[si(n)] | bm(n)) != 0;
  }

  /// Set a given bit.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  YAT_BITMAP_CONSTEXPR void set(size_t n) {
    auto& val = _storage[si(n)];
    val = val | bm(n);
  }

  /// Set a range of bits.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  YAT_BITMAP_CONSTEXPR void set(size_t start, size_t count) {
    while (count > 0) {
      if (bi(start) == 0 && count >= storage_bits) {
        // We're dealing the the entire element, so set all the bits
        _storage[si(start)] =
            std::numeric_limits<storage_type::value_type>::max();

        start += storage_bits;
        count -= storage_bits;
        continue;
      }

      set(start);
      start++;
      count--;
    }
  }

  /// Clear a given bit.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  YAT_BITMAP_CONSTEXPR void clear(size_t n) noexcept {
    auto& val = _storage[si(n)];
    val = val & ~bm(n);
  }

  /// Clear a range of bits.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  YAT_BITMAP_CONSTEXPR void clear(size_t start, size_t count) {
    while (count > 0) {
      if (bi(start) == 0 && count >= storage_bits) {
        // We're dealing the the entire element, so clear all the bits
        _storage[si(start)] = 0;

        start += storage_bits;
        count -= storage_bits;
        continue;
      }

      clear(start);
      start++;
      count--;
    }
  }

  /// Return the count of bits in the set
  constexpr size_t count() const noexcept { return _count; }

  /// Resize the bitset
  YAT_BITMAP_CONSTEXPR void resize(size_t n) {
    _storage.resize(storage_size(n));
    _count = n;
  }

 private:
  std::vector<storage_type> _storage{};  ///< Underlying bit storage
  size_t _count{};                       ///< Number of bits in bitset
};

}  // namespace yat
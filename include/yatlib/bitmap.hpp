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

#include "bit.hpp"
#include "endian.hpp"
#include "span.hpp"

namespace yat {

/// `bitmap` represents a sequence of bits that can be manipulated efficiently.
///
/// This is similar to std::vector<bool>, but the layout of the bits in memory
/// is well defined.
class bitmap {
  using storage_type = yat::little_uint64_t;
  static constexpr uint64_t storage_bits =
      std::numeric_limits<storage_type::value_type>::digits;

  /// Calculate the number of storage ints we need to store n bits
  static constexpr uint64_t storage_size(uint64_t n) noexcept {
    return (n + storage_bits - 1) / storage_bits;
  }

  /// Calculate the storage index
  static constexpr uint64_t si(uint64_t n) noexcept { return n / storage_bits; }

  /// Calculate the bit index
  static constexpr uint64_t bi(uint64_t n) noexcept { return n % storage_bits; }

  /// Calculate the bitmask for an index
  static constexpr storage_type::value_type bm(uint64_t n) noexcept {
    return 1ULL << bi(n);
  }

 public:
  /// Create an empty bitmap
  bitmap() noexcept {};  // clang 5 had a bug when using "= default" here

  /// Create a bitmap with `n` unset bits
  explicit bitmap(uint64_t n) : _storage(storage_size(n)), _count{n} {}

  /// Access a given bit.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  bool operator[](uint64_t n) const { return (_storage[si(n)] | bm(n)) != 0; }

  /// Set a given bit.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  void set(uint64_t n) {
    auto& val = _storage[si(n)];
    val = val | bm(n);
  }

  /// Set a range of bits.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  void set(uint64_t start, uint64_t count) {
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
  void clear(uint64_t n) noexcept {
    auto& val = _storage[si(n)];
    val = val & ~bm(n);
  }

  /// Clear a range of bits.  No bounds checking is performed and accessing an
  /// invalid index is undefined behavior.
  void clear(uint64_t start, uint64_t count) {
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
  constexpr uint64_t count() const noexcept { return _count; }

  /// Resize the bitset
  void resize(uint64_t n) {
    _storage.resize(storage_size(n));
    _count = n;
  }

 private:
  std::vector<storage_type> _storage{};  ///< Underlying bit storage
  uint64_t _count{};                     ///< Number of bits in bitset

  friend class bitmap_scanner;
};

/// A bitmap scanner is used to scan bitmaps for ranges of set or unset bits.
///
/// It assumes that bitmaps are stored as an array of bytes that count bits from
/// LSB->MSB.
class bitmap_scanner {
  /// Special marker that is returned when there are no bits left to scan
  static constexpr uint64_t no_bits_left = std::numeric_limits<uint64_t>::max();

  //
  //  Rather than reading one byte at a time, we can use little endian unsigned
  //  integers of any size and the byte reordering will work as expected.
  //
  using storage_type = yat::little_scalar<uintmax_t>;
  static constexpr auto storage_bits =
      std::numeric_limits<storage_type::value_type>::digits;

  /// Calculates the needed array size to hold a certain amount of bits
  static constexpr uint64_t cas(uint64_t block_count) noexcept {
    return (block_count + storage_bits - 1) / storage_bits;
  }

  /// An iterator for bitmap_scanner that iterates through the scanned ranges
  class iterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = struct {
      uint64_t start;  /// start of the range
      uint64_t count;  /// number of elements in the range
    };
    using difference_type = void;  // No meaningful way of taking difference
    using pointer = const value_type*;
    using reference = const value_type&;

    /// Construct an empty iterator (this will compare with end())
    constexpr iterator() noexcept {};  // clang 5 had a bug when using
                                       // "= default" here

    /// Construct an iterator from a chunk bitmap
    explicit iterator(const bitmap_scanner* bm)
        : _bm{bm}, _find_set{_bm->_scan_set} {
      next();
    }

    /// Equality operator
    friend bool operator==(const iterator& lhs, const iterator& rhs) noexcept {
      return (lhs._bm == rhs._bm) && (lhs._next_block == rhs._next_block);
    }

    /// Inequality operator
    friend bool operator!=(const iterator& lhs, const iterator& rhs) noexcept {
      return (lhs._bm != rhs._bm) || (lhs._next_block != rhs._next_block);
    }

    /// Dereference operator
    reference operator*() const noexcept { return _range; }

    /// Pointer dereference operator
    pointer operator->() const noexcept { return &_range; }

    /// Prefix increment operator
    iterator& operator++() { return next(); }

    /// Postfix increment operator
    iterator operator++(int) {
      iterator copy{*this};

      operator++();

      return copy;
    }

   protected:
    /// Cache the next set of bitmap data to read
    void cache_next() noexcept {
      // Fetch the next set of bits into the cache
      _cache = _bm->_bits[_next_block / _bm->storage_bits];

      // If we're scanning for unset bits then invert the bits since we actually
      // only do logic to scan for ones
      if (!_find_set) {
        _cache = ~_cache;
      }
    }

    /// Toggles the scanning mode
    void toggle_mode() noexcept {
      _find_set = !_find_set;
      _cache = ~_cache;
    }

    /// Scan for the next set bit
    uint64_t scan() noexcept {
      while (_next_block < _bm->_block_count) {
        // Calculate the bit we're starting our scan
        const auto i = _next_block % _bm->storage_bits;

        // If we're scanning the first bit, we need to cache the bits
        if (i == 0) {
          cache_next();

          // If there are no bits set then there's nothing to scan for, so let's
          // move on.
          if (_cache == 0) {
            _next_block += _bm->storage_bits;
            continue;
          }
        }

        // Mask the already fetched baclues and count the number of trailing
        // zero bits
        const auto c = yat::countr_zero((_cache >> i) << i);

        // If c is the number of bits, then there are no more set bits
        // and we need to move on to scan the next set
        if (c == _bm->storage_bits) {
          _next_block += _bm->storage_bits - i;
          continue;
        }

        // Adjust the next block for the next scan
        _next_block += static_cast<uint64_t>(c) + 1 - i;

        // If the last black is within range then return it
        if (const uint64_t next = _next_block - 1; next < _bm->_block_count) {
          return next;
        }

        return no_bits_left;
      }

      return no_bits_left;
    }

    /// Get the next range
    iterator& next() noexcept {
      // Get the start of the next range
      const uint64_t s = scan();

      // If there's no start then there are no more ranges and we need to set
      // ourself as the end interator
      if (s == no_bits_left) {
        *this = {};
        return (*this);
      }

      // Toggle the scan mode to look for the next type of bit
      toggle_mode();

      // Get the end of the range
      uint64_t e = scan();

      // Toggle the scan mode to look for the next type of bit
      toggle_mode();

      // If there's no end then we set the end of the range to the end of the
      // bitmap
      if (e == no_bits_left) {
        e = _bm->_block_count;
      }

      _range = {s, e - s};

      return (*this);
    }

   private:
    const bitmap_scanner* _bm{};  ///< Unowned pointer to bitmap
    uint64_t _next_block{};       ///< The next block number to be scanned
    bool _find_set{};             ///< The current scanning mode
    bitmap_scanner::storage_type _cache{};  ///< Cached data
    value_type _range{};                    ///< The current range
  };

 public:
  /// Creates a bitmap scanner from raw data
  ///
  /// \param data The bitmap data to scan
  /// \param count The number of bits to scan in the bitmap
  /// \param scan_set Indicates that we're scanning for ranges of set bits
  bitmap_scanner(const void* data, uint64_t count,
                 bool scan_set = true) noexcept
      : _scan_set{scan_set},
        _block_count{count},
        _bits{static_cast<const storage_type*>(data), cas(count)} {}

  /// Creates a bitmap scanner for a given bitmap
  bitmap_scanner(const bitmap& bitmap, bool scan_set = true) noexcept
      : bitmap_scanner(bitmap._storage.data(), bitmap._count, scan_set) {}

  /// Returns an iterator to the start of the ranges
  [[nodiscard]] iterator begin() const noexcept { return iterator{this}; }

  /// Returns an iterator to the end of the ranges
  [[nodiscard]] iterator end() const noexcept { return {}; }

  /// Returns true if the scanner has enough data to do scanning
  [[nodiscard]] bool is_valid() const noexcept { return !_bits.empty(); }

  /// Returns true if the scanner has enough data to do scanning
  [[nodiscard]] explicit operator bool() const noexcept { return is_valid(); }

 private:
  bool _scan_set{};         ///< True if scanning for ranges of set bits
  uint64_t _block_count{};  ///< The number of bits that we're scanning for
  yat::span<const storage_type> _bits{};  ///< The view into the data
};

}  // namespace yat
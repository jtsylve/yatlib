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

#if __has_include("span")
  #include <span>
#endif

#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>

#include "memory.hpp"

/////////////////////////////////////////
// P0122R3 - https://wg21.link/P0122R3 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_span) && __cpp_lib_span >= 202002L
  #define YAT_INTERNAL_USE_STD_SPAN
#endif

#ifdef YAT_INTERNAL_USE_STD_SPAN

namespace yat {
using std::dynamic_extent;
using std::span;
}  // namespace yat

#else

namespace yat {

/// yat::dynamic_extent is a constant of type std::size_t that is used to
/// differentiate yat::span of static and dynamic extent.
inline constexpr std::size_t dynamic_extent =
    std::numeric_limits<std::size_t>::max();

/// The class template span describes an object that can refer to a contiguous
/// sequence of objects with the first element of the sequence at position zero.
/// A span can either have a static extent, in which case the number of elements
/// in the sequence is known and encoded in the type, or a dynamic extent.
template <typename ElementType, std::size_t Extent = dynamic_extent>
class span {
 public:
  // constants and types
  using element_type = ElementType;
  using value_type = std::remove_cv_t<ElementType>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = element_type *;
  using const_pointer = const element_type *;
  using reference = element_type &;
  using const_reference = const element_type &;
  class iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;

  static constexpr size_type extent = Extent;

  /// Constructs an empty span whose data() == nullptr and size() == 0.
  ///
  /// This overload only participates in overload resolution if extent == 0 ||
  /// extent == std::dynamic_extent.
  template <std::size_t S = Extent,
            typename = std::enable_if_t<
                std::disjunction_v<std::bool_constant<S == 0>,
                                   std::bool_constant<S == dynamic_extent> > > >
  constexpr span() noexcept {}

  /// Constructs a span that is a view over the range [first, first + count);
  /// the resulting span has data() == yat::to_address(first) and size() ==
  /// count.
  ///
  /// The behavior is undefined if [first, first + count) is not a valid range,
  /// if It does not actually model contiguous_iterator, or if extent !=
  /// std::dynamic_extent && count != extent.
  template <typename It>
  constexpr span(It first, size_type count) noexcept
      : _count{count}, _data{yat::to_address(first)} {
    if constexpr (Extent != dynamic_extent) {
      assert(_count == Extent);
    }
  }

  /// Constructs a span that is a view over the range [first, last); the
  /// resulting span has data() == yat::to_address(first) and size() ==
  /// last-first.
  ///
  /// The behavior is undefined if [first, last) is not a valid range, if It
  /// does not actually model contiguous_iterator, if End does not actually
  /// model sized_sentinel_for for It, or if extent != std::dynamic_extent &&
  /// last-first != extent.
  template <typename It, typename End>
  constexpr span(It first, End last) noexcept
      : _count{last - first}, _data{yat::to_address(first)} {
    if constexpr (Extent != dynamic_extent) {
      assert(_count == Extent);
    }
  }

  /// Constructs a span that is a view over the array arr; the resulting span
  /// has size() == N and data() == std::data(arr).
  template <std::size_t N>  // cppcheck-suppress noExplicitConstructor
  constexpr span(element_type (&arr)[N]) noexcept
      : _count{N}, _data{std::data(arr)} {
    static_assert(Extent == dynamic_extent || N == Extent);
  }

  /// Constructs a span that is a view over the array arr; the resulting span
  /// has size() == N and data() == std::data(arr).
  template <typename T,
            std::size_t N>  // cppcheck-suppress noExplicitConstructor
  constexpr span(std::array<T, N> &arr) noexcept
      : _count{N}, _data{static_cast<pointer>(std::data(arr))} {
    static_assert(Extent == dynamic_extent || N == Extent);
  }

  /// Constructs a span that is a view over the array arr; the resulting span
  /// has size() == N and data() == std::data(arr).
  template <typename T,
            std::size_t N>  // cppcheck-suppress noExplicitConstructor
  constexpr span(const std::array<T, N> &arr) noexcept
      : _count{N}, _data{static_cast<pointer>(std::data(arr))} {
    static_assert(Extent == dynamic_extent || N == Extent);
  }

  /// Returns the number of elements in the span.
  [[nodiscard]] constexpr size_type size() const noexcept { return _count; }

  /// Returns the size of the sequence in bytes.
  [[nodiscard]] constexpr size_type size_bytes() const noexcept {
    return _count * sizeof(element_type);
  }

  /// Checks if the span is empty.
  [[nodiscard]] constexpr bool empty() const noexcept { return (_count == 0); }

  /// Returns a pointer to the beginning of the sequence.
  [[nodiscard]] constexpr pointer data() const noexcept { return _data; }

 private:
  size_type _count{};
  pointer _data{};
};

#endif  // YAT_INTERNAL_USE_STD_SPAN

}  // namespace yat

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_SPAN

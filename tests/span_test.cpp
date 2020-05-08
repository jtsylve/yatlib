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
#include <type_traits>
#include <yatlib/span.hpp>

#include "common.hpp"

// Tests adapted from
// https://github.com/microsoft/STL/blob/master/tests/std/tests/P0122R7_span/test.cpp

TEST_CASE("dynamic_extent", "[span][dynamic_extent]") {
  STATIC_REQUIRE(
      std::is_same_v<decltype(yat::dynamic_extent), const std::size_t>);
  STATIC_REQUIRE(yat::dynamic_extent == static_cast<std::size_t>(-1));
}

struct Base {};
struct Derived : Base {};
static_assert(std::is_convertible_v<Derived*, Base*>);
static_assert(!std::is_convertible_v<Derived (*)[], Base (*)[]>);

struct NonRange {};

template <typename T, bool Borrowed = false>
struct BasicRange {
  T elements[3]{};

  constexpr size_t size() const { return 3; }

  constexpr T* data() { return elements; }

  constexpr const T* data() const { return elements; }

  constexpr T* begin() { return elements; }

  constexpr const T* begin() const { return elements; }

  constexpr T* end() { return elements + 3; }

  constexpr const T* end() const { return elements + 3; }
};

using ContiguousSizedRange = BasicRange<int>;

// Not truly a model of borrowed_range; this is a convenient fiction for testing
// purposes.
using BorrowedContiguousSizedRange = BasicRange<int, true>;

// template <typename T, size_t Extent = yat::dynamic_extent>
// constexpr void FunctionTakingSpan(type_identity_t<yat::span<T, Extent>>) {}

template <typename U, typename = void>
inline constexpr bool AsWritableBytesCompilesFor = false;

// template <typename U>
// inline constexpr bool AsWritableBytesCompilesFor<
//   U, void_t<decltype(yat::as_writable_bytes(declval<U>()))>> = true;

TEST_CASE("span", "[span][span]") {
  STATIC_REQUIRE(
      std::is_same_v<decltype(yat::span<int>::extent), const std::size_t>);
  STATIC_REQUIRE(yat::span<int>::extent == yat::dynamic_extent);

  STATIC_REQUIRE(
      std::is_same_v<decltype(yat::span<int, 3>::extent), const size_t>);
  STATIC_REQUIRE(yat::span<int, 3>::extent == 3);

  STATIC_REQUIRE(std::is_same_v<yat::span<int>::element_type, int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::value_type, int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::size_type, size_t>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::difference_type, ptrdiff_t>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::pointer, int*>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::const_pointer, const int*>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::reference, int&>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int>::const_reference, const int&>);

  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::element_type, int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::value_type, int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::size_type, size_t>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::difference_type, ptrdiff_t>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::pointer, int*>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::const_pointer, const int*>);
  STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::reference, int&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<int, 3>::const_reference, const int&>);

  STATIC_REQUIRE(std::is_same_v<yat::span<const int>::element_type, const int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int>::value_type, int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int>::size_type, size_t>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int>::difference_type, ptrdiff_t>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int>::pointer, const int*>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int>::const_pointer, const int*>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int>::reference, const int&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int>::const_reference, const int&>);

  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int, 3>::element_type, const int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int, 3>::value_type, int>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int, 3>::size_type, size_t>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int, 3>::difference_type, ptrdiff_t>);
  STATIC_REQUIRE(std::is_same_v<yat::span<const int, 3>::pointer, const int*>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int, 3>::const_pointer, const int*>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int, 3>::reference, const int&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::span<const int, 3>::const_reference, const int&>);

#if 0
    // The iterators are thoroughly tested by P0896R4_ranges_range_machinery
    STATIC_REQUIRE(
        std::is_same_v<iterator_traits<yat::span<int>::iterator>::pointer,
    int*>); STATIC_REQUIRE(std::is_same_v<yat::span<int>::reverse_iterator,
                                  reverse_iterator<yat::span<int>::iterator>>);

    STATIC_REQUIRE(
        std::is_same_v<iterator_traits<yat::span<int, 3>::iterator>::pointer,
                       int*>);
    STATIC_REQUIRE(std::is_same_v<yat::span<int, 3>::reverse_iterator,
                                  reverse_iterator<yat::span<int,
    3>::iterator>>);

    STATIC_REQUIRE(
        std::is_same_v<iterator_traits<yat::span<const int>::iterator>::pointer,
                       const int*>);
    STATIC_REQUIRE(
        std::is_same_v<yat::span<const int>::reverse_iterator,
                       reverse_iterator<yat::span<const int>::iterator>>);

    STATIC_REQUIRE(std::is_same_v<
                   iterator_traits<yat::span<const int, 3>::iterator>::pointer,
                   const int*>);
    STATIC_REQUIRE(
        std::is_same_v<yat::span<const int, 3>::reverse_iterator,
                       reverse_iterator<yat::span<const int, 3>::iterator>>);
#endif

  // For performance, our implementation provides an additional guarantee beyond
  // the Standard
  // that span and its iterator types are trivially copyable.
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<int>>);
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<int, 3>>);
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<const int>>);
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<const int, 3>>);

#if 0
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<int>::iterator>);
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<int, 3>::iterator>);
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<const int>::iterator>);
  STATIC_REQUIRE(std::is_trivially_copyable_v<yat::span<const int,3>::iterator>);
#endif

  {
    STATIC_REQUIRE(std::is_nothrow_default_constructible_v<yat::span<int>>);
    STATIC_REQUIRE(std::is_nothrow_default_constructible_v<yat::span<int, 0>>);
    STATIC_REQUIRE(!std::is_default_constructible_v<yat::span<int, 3>>);

    yat::span<int> sp_dyn;
    REQUIRE(sp_dyn.data() == nullptr);
    REQUIRE(sp_dyn.size() == 0);
    REQUIRE(sp_dyn.size_bytes() == 0);
    REQUIRE(sp_dyn.empty());

    yat::span<int, 0> sp_zero;
    REQUIRE(sp_zero.data() == nullptr);
    REQUIRE(sp_zero.size() == 0);
    REQUIRE(sp_zero.size_bytes() == 0);
    REQUIRE(sp_zero.empty());

    // FunctionTakingSpan<int>({});
    // FunctionTakingSpan<int, 0>({});
  }

  int arr[3]{10, 20, 30};
  std::array<int, 3> stl{{100, 200, 300}};
  std::array<int*, 3> stl_nullptr{{nullptr, nullptr, nullptr}};

  {
    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<int>, int*,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<int>, int*,
                                                   uint16_t>);  // strengthened
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int>, const int*, size_t>);
    STATIC_REQUIRE(!std::is_constructible_v<yat::span<int>, double*, size_t>);
    STATIC_REQUIRE(!std::is_constructible_v<yat::span<int>, size_t, size_t>);

    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<int, 3>, int*,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<int, 3>, int*,
                                                   uint16_t>);  // strengthened
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int, 3>, const int*, size_t>);
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int, 3>, double*, size_t>);
    STATIC_REQUIRE(!std::is_constructible_v<yat::span<int, 3>, size_t, size_t>);

    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<const int>, int*,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(
        std::is_nothrow_constructible_v<yat::span<const int>, const int*,
                                        size_t>);  // strengthened

    yat::span<int> sp_dyn_a(arr, 3);
    yat::span<int> sp_dyn_b(std::begin(arr), 3);
    REQUIRE(sp_dyn_a.data() == std::begin(arr));
    REQUIRE(sp_dyn_b.data() == std::begin(arr));
    REQUIRE(sp_dyn_a.size() == 3);
    REQUIRE(sp_dyn_b.size() == 3);

    yat::span<int, 3> sp_three_a(arr, 3);
    yat::span<int, 3> sp_three_b(std::begin(arr), 3);
    REQUIRE(sp_three_a.data() == std::begin(arr));
    REQUIRE(sp_three_b.data() == std::begin(arr));
    REQUIRE(sp_three_a.size() == 3);
    REQUIRE(sp_three_b.size() == 3);

    yat::span<const int> sp_const_a(arr, 3);
    yat::span<const int> sp_const_b(std::begin(arr), 3);
    REQUIRE(sp_const_a.data() == std::begin(arr));
    REQUIRE(sp_const_b.data() == std::begin(arr));
    REQUIRE(sp_const_a.size() == 3);
    REQUIRE(sp_const_b.size() == 3);

    yat::span<const int> sp_const_c(std::as_const(arr), 3);
    yat::span<const int> sp_const_d(std::cbegin(arr), 3);
    REQUIRE(sp_const_c.data() == std::begin(arr));
    REQUIRE(sp_const_d.data() == std::begin(arr));
    REQUIRE(sp_const_c.size() == 3);
    REQUIRE(sp_const_d.size() == 3);
#if 0
    FunctionTakingSpan<int>({arr, 3});
    FunctionTakingSpan<int>({std::begin(arr), 3});
    FunctionTakingSpan<const int>({arr, 3});
    FunctionTakingSpan<const int>({std::begin(arr), 3});
    FunctionTakingSpan<const int>({as_const(arr), 3});
    FunctionTakingSpan<const int>({std::cbegin(arr), 3});
#endif

    STATIC_REQUIRE(std::is_same_v<decltype(yat::span{arr, 3}), yat::span<int>>);
    STATIC_REQUIRE(std::is_same_v<decltype(yat::span{std::begin(arr), 3}),
                                  yat::span<int>>);

    STATIC_REQUIRE(std::is_same_v<decltype(yat::span{std::as_const(arr), 3}),
                                  yat::span<const int>>);
    STATIC_REQUIRE(std::is_same_v<decltype(yat::span{std::cbegin(arr), 3}),
                                  yat::span<const int>>);

    STATIC_REQUIRE(
        std::is_nothrow_constructible_v<yat::span<int>, array<int, 3>::iterator,
                                        size_t>);  // strengthened
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int>, array<int, 3>::const_iterator,
                                 size_t>);
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int>, array<double, 3>::iterator,
                                 size_t>);

    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<int, 3>,
                                                   array<int, 3>::iterator,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int, 3>,
                                 array<int, 3>::const_iterator, size_t>);
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<int, 3>, array<double, 3>::iterator,
                                 size_t>);

    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<const int>,
                                                   array<int, 3>::iterator,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(
        std::is_nothrow_constructible_v<yat::span<const int>,
                                        array<int, 3>::const_iterator,
                                        size_t>);  // strengthened

    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<Base>, Base*,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<Base, 3>, Base*,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(!std::is_constructible_v<yat::span<Base>, Derived*, size_t>);
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<Base, 3>, Derived*, size_t>);

    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<Base>,
                                                   array<Base, 3>::iterator,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(std::is_nothrow_constructible_v<yat::span<Base, 3>,
                                                   array<Base, 3>::iterator,
                                                   size_t>);  // strengthened
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<Base>, array<Derived, 3>::iterator,
                                 size_t>);
    STATIC_REQUIRE(
        !std::is_constructible_v<yat::span<Base, 3>,
                                 array<Derived, 3>::iterator, size_t>);

    yat::span<int> sp_dyn_x(stl.begin(), 3);
    REQUIRE(sp_dyn_x.data() == stl.data());
    REQUIRE(sp_dyn_x.size() == 3);

    yat::span<int, 3> sp_three_x(stl.begin(), 3);
    REQUIRE(sp_three_x.data() == stl.data());
    REQUIRE(sp_three_x.size() == 3);

    yat::span<const int> sp_const_x(stl.begin(), 3);
    REQUIRE(sp_const_x.data() == stl.data());
    REQUIRE(sp_const_x.size() == 3);

    yat::span<const int> sp_const_y(stl.cbegin(), 3);
    REQUIRE(sp_const_y.data() == stl.data());
    REQUIRE(sp_const_y.size() == 3);

    FunctionTakingSpan<int>({stl.begin(), 3});
    FunctionTakingSpan<const int>({stl.begin(), 3});
    FunctionTakingSpan<const int>({stl.cbegin(), 3});

    STATIC_REQUIRE(
        std::is_same_v<decltype(yat::span{stl.begin(), 3}), yat::span<int>>);
    STATIC_REQUIRE(std::is_same_v<decltype(yat::span{stl.cbegin(), 3}),
                                  yat::span<const int>>);
  }
}

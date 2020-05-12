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
#include <yatlib/iterator.hpp>

#include "common.hpp"

template <typename T, typename = void>
constexpr bool has_it_difference_type = false;

template <typename T>
constexpr bool has_it_difference_type<
    T, std::void_t<typename yat::incrementable_traits<T>::difference_type>> =
    true;

// Type with no difference type or operator
struct A {};

// Type with no difference type or operator, but is specialized
struct B {};

template <>
struct yat::incrementable_traits<B> {
  using difference_type = float;
};

// Type with difference type, but no difference operator
struct C {
  using difference_type = int;
};

// Type with difference type, but and difference operator
struct D {
  using difference_type = int;

  friend constexpr float operator-(const D &, const D &) noexcept;
};

template <typename T, typename Expected>
static void test_difference_type() {
  STATIC_REQUIRE(
      std::is_same_v<typename yat::incrementable_traits<T>::difference_type,
                     Expected>);
}

TEST_CASE("incrementable_traits", "[iterator][incrementable_traits]") {
  STATIC_REQUIRE_FALSE(has_it_difference_type<A>);
  STATIC_REQUIRE(has_it_difference_type<B>);
  STATIC_REQUIRE(has_it_difference_type<C>);
  STATIC_REQUIRE(has_it_difference_type<D>);

  STATIC_REQUIRE_FALSE(has_it_difference_type<const A>);
  STATIC_REQUIRE(has_it_difference_type<const B>);
  STATIC_REQUIRE(has_it_difference_type<const C>);
  STATIC_REQUIRE(has_it_difference_type<const D>);

  STATIC_REQUIRE(has_it_difference_type<A *>);
  STATIC_REQUIRE(has_it_difference_type<B *>);
  STATIC_REQUIRE(has_it_difference_type<C *>);
  STATIC_REQUIRE(has_it_difference_type<D *>);

  STATIC_REQUIRE(has_it_difference_type<int>);
  STATIC_REQUIRE(has_it_difference_type<int *>);

  test_difference_type<B, float>();
  test_difference_type<C, int>();
  test_difference_type<D, int>();

  test_difference_type<const B, float>();
  test_difference_type<const C, int>();
  test_difference_type<const D, int>();

  test_difference_type<A *, std::ptrdiff_t>();
  test_difference_type<B *, std::ptrdiff_t>();
  test_difference_type<C *, std::ptrdiff_t>();
  test_difference_type<D *, std::ptrdiff_t>();

  test_difference_type<int, int>();
  test_difference_type<int *, std::ptrdiff_t>();
}

template <class T>
struct dereferences_to {
  using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
  T operator*() const;
};

TEST_CASE("iter_reference_t", "[iterator][iter_reference_t]") {
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int *>, int &>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<int const *>, int const &>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<int volatile *>, int volatile &>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int const volatile *>,
                                int const volatile &>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int[]>, int &>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int[4]>, int &>);

  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int>>, int>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int &>>, int &>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int &&>>, int &&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int const &>>,
                     int const &>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int const &&>>,
                     int const &&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int volatile &>>,
                     int volatile &>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int volatile &&>>,
                     int volatile &&>);
  STATIC_REQUIRE(std::is_same_v<
                 yat::iter_reference_t<dereferences_to<int const volatile &>>,
                 int const volatile &>);
  STATIC_REQUIRE(std::is_same_v<
                 yat::iter_reference_t<dereferences_to<int const volatile &&>>,
                 int const volatile &&>);

  struct with_bogus_typedefs : dereferences_to<int &> {
    using value_type = double;
    using reference = double;
  };

  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<with_bogus_typedefs>, int &>);
}

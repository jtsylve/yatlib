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
#include <yatlib/array.hpp>

#include "common.hpp"

// Tests adapted from
// https://github.com/microsoft/STL/blob/master/tests/std/tests/P0325R4_to_array/test.cpp

template <typename T, typename E, std::size_t N>
static void assert_equal(T&& actual, const std::array<E, N>& expected) {
  STATIC_REQUIRE(std::is_same_v<T, std::array<E, N>>);

  for (auto it1 = cbegin(actual), it2 = cbegin(expected); it1 != cend(actual);
       ++it1, ++it2) {
    REQUIRE(*it1 == *it2);
  }
}

struct A {
  int i{};
  double d{};

  [[nodiscard]] friend constexpr bool operator==(const A& lhs,
                                                 const A& rhs) noexcept {
    return (lhs.i == rhs.i) && (lhs.d == rhs.d);
  }
};

TEST_CASE("to_array", "[array][to_array]") {
  int a[3]{};
  const int c_a[3]{};

  assert_equal(yat::to_array("meow"),
               std::array<char, 5>{'m', 'e', 'o', 'w', '\0'});
  assert_equal(yat::to_array({1, 2}), std::array<int, 2>{1, 2});
  assert_equal(yat::to_array({1L, 2L}), std::array<long, 2>{1, 2});
  assert_equal(yat::to_array(a), std::array<int, 3>{0, 0, 0});
  assert_equal(yat::to_array(c_a), std::array<int, 3>{0, 0, 0});
  assert_equal(yat::to_array(std::move(a)), std::array<int, 3>{0, 0, 0});
  assert_equal(yat::to_array(std::move(c_a)), std::array<int, 3>{0, 0, 0});
  assert_equal(yat::to_array<A>({{3, 0.1}}), std::array<A, 1>{{{3, 0.1}}});
}

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

template <class T>
struct dereferences_to {
  using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
  T operator*() const;
};

TEST_CASE("iter_reference_t", "[iterator][iter_reference_t]") {
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int*>, int&>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int const*>, int const&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<int volatile*>, int volatile&>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int const volatile*>,
                                int const volatile&>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int[]>, int&>);
  STATIC_REQUIRE(std::is_same_v<yat::iter_reference_t<int[4]>, int&>);

  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int>>, int>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int&>>, int&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int&&>>, int&&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int const&>>,
                     int const&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int const&&>>,
                     int const&&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int volatile&>>,
                     int volatile&>);
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<dereferences_to<int volatile&&>>,
                     int volatile&&>);
  STATIC_REQUIRE(std::is_same_v<
                 yat::iter_reference_t<dereferences_to<int const volatile&>>,
                 int const volatile&>);
  STATIC_REQUIRE(std::is_same_v<
                 yat::iter_reference_t<dereferences_to<int const volatile&&>>,
                 int const volatile&&>);

  struct with_bogus_typedefs : dereferences_to<int&> {
    using value_type = double;
    using reference = double;
  };
  STATIC_REQUIRE(
      std::is_same_v<yat::iter_reference_t<with_bogus_typedefs>, int&>);
}

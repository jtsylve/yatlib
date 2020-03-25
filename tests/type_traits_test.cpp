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
#include <variant>
#include <yatlib/type_traits.hpp>

#include "common.hpp"

TEST_CASE("is_variant_member", "[type_traits][is_variant_member]") {
  using vtype = std::variant<std::monostate, char, int, unsigned int, long,
                             unsigned long long>;

  STATIC_REQUIRE(yat::is_variant_member_v<std::monostate, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<char, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<int, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<unsigned int, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<long, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<unsigned long long, vtype>);

  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<float, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<double, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<long long, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<int *, vtype>);

  STATIC_REQUIRE(std::is_same_v<yat::is_variant_member_t<std::monostate, vtype>,
                                std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<char, vtype>, std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<int, vtype>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_member_t<unsigned int, vtype>,
                                std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<long, vtype>, std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<unsigned long long, vtype>,
                     std::true_type>);

  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<float, vtype>, std::false_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<double, vtype>, std::false_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_member_t<long long, vtype>,
                                std::false_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_member_t<int *, vtype>, std::false_type>);
}
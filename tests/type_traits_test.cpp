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
#include <yatlib/features.hpp>
#include <yatlib/type_traits.hpp>
#include <yatlib/variant.hpp>

#include "common.hpp"

TEST_CASE("is_variant_member", "[type_traits][is_variant_member]") {
  using vtype = yat::variant<yat::monostate, char, int, unsigned int, long,
                             unsigned long long>;

  STATIC_REQUIRE(yat::is_variant_member_v<yat::monostate, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<char, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<int, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<unsigned int, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<long, vtype>);
  STATIC_REQUIRE(yat::is_variant_member_v<unsigned long long, vtype>);

  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<float, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<double, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<long long, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_member_v<int *, vtype>);

  STATIC_REQUIRE(std::is_same_v<yat::is_variant_member_t<yat::monostate, vtype>,
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

TEST_CASE("is_one_of", "[type_traits][is_one_of]") {
  STATIC_REQUIRE(yat::is_one_of_v<int, char, int, float, double>);
  STATIC_REQUIRE(std::is_same_v<yat::is_one_of_t<int, char, int, float, double>,
                                std::true_type>);

  STATIC_REQUIRE_FALSE(yat::is_one_of_v<unsigned, char, int, float, double>);
  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_one_of_t<unsigned, char, int, float, double>,
                     std::true_type>);
}

TEST_CASE("is_char_type", "[type_traits][is_char_type]") {
  STATIC_REQUIRE(yat::is_char_type_v<char>);
  STATIC_REQUIRE(yat::is_char_type_v<wchar_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char16_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char32_t>);
  STATIC_REQUIRE(yat::is_char_type_v<unsigned char>);
  STATIC_REQUIRE(yat::is_char_type_v<wchar_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char16_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char32_t>);

#ifdef YAT_SUPPORTS_CPP20
  STATIC_REQUIRE(yat::is_char_type_v<char8_t>);
#endif

  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<char>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<wchar_t>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<char16_t>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<char32_t>, std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_char_type_t<unsigned char>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<wchar_t>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<char16_t>, std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<char32_t>, std::true_type>);

#ifdef YAT_SUPPORTS_CPP20
  STATIC_REQUIRE(std::is_same_v<yat::is_char_type_t<char8_t>, std::true_type>);
#endif

  STATIC_REQUIRE(yat::is_char_type_v<char>);
  STATIC_REQUIRE(yat::is_char_type_v<wchar_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char16_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char32_t>);
  STATIC_REQUIRE(yat::is_char_type_v<unsigned char>);
  STATIC_REQUIRE(yat::is_char_type_v<wchar_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char16_t>);
  STATIC_REQUIRE(yat::is_char_type_v<char32_t>);

#ifdef YAT_SUPPORTS_CPP20
  STATIC_REQUIRE(yat::is_char_type_v<char8_t>);
#endif

  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_char_type_t<int>, std::true_type>);
  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_char_type_t<float>, std::true_type>);
  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_char_type_t<double>, std::true_type>);

  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_char_type_t<int>, std::true_type>);
  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_char_type_t<float>, std::true_type>);
  STATIC_REQUIRE_FALSE(
      std::is_same_v<yat::is_char_type_t<double>, std::true_type>);
}

struct A {
  constexpr void *operator*() const noexcept;
};

TEST_CASE("is_derefenceable", "[type_traits][is_derefenceable]") {
  STATIC_REQUIRE(yat::is_dereferencable_v<int *>);
  STATIC_REQUIRE_FALSE(yat::is_dereferencable_v<int>);
  STATIC_REQUIRE(yat::is_dereferencable_v<A>);
  STATIC_REQUIRE(yat::is_dereferencable_v<int (*)(int)>);
}

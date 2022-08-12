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

//
// Many of these tests are taken directly from
// https://github.com/microsoft/STL/blob/1ece8a0352327397997c3f4b649a228c66da3ce1/tests/std/tests/VSO_0000000_type_traits/test.cpp
//

#include <yatlib/features.hpp>
#include <yatlib/type_traits.hpp>
#include <yatlib/variant.hpp>

#include "common.hpp"

TEST_CASE("is_variant_alternative", "[type_traits][is_variant_alternative]") {
  using vtype = yat::variant<yat::monostate, char, int, unsigned int, long,
                             unsigned long long>;

  STATIC_REQUIRE(yat::is_variant_alternative_v<yat::monostate, vtype>);
  STATIC_REQUIRE(yat::is_variant_alternative_v<char, vtype>);
  STATIC_REQUIRE(yat::is_variant_alternative_v<int, vtype>);
  STATIC_REQUIRE(yat::is_variant_alternative_v<unsigned int, vtype>);
  STATIC_REQUIRE(yat::is_variant_alternative_v<long, vtype>);
  STATIC_REQUIRE(yat::is_variant_alternative_v<unsigned long long, vtype>);

  STATIC_REQUIRE_FALSE(yat::is_variant_alternative_v<float, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_alternative_v<double, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_alternative_v<long long, vtype>);
  STATIC_REQUIRE_FALSE(yat::is_variant_alternative_v<int *, vtype>);

  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_alternative_t<yat::monostate, vtype>,
                     std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<char, vtype>,
                                std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<int, vtype>,
                                std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_alternative_t<unsigned int, vtype>,
                     std::true_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<long, vtype>,
                                std::true_type>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_variant_alternative_t<unsigned long long, vtype>,
                     std::true_type>);

  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<float, vtype>,
                                std::false_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<double, vtype>,
                                std::false_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<long long, vtype>,
                                std::false_type>);
  STATIC_REQUIRE(std::is_same_v<yat::is_variant_alternative_t<int *, vtype>,
                                std::false_type>);
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

#ifdef YAT_SUPPORTS_CHAR8_T
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

template <typename T>
constexpr bool test_type_identity() {
  static_assert(std::is_same_v<T, typename yat::type_identity<T>::type>);
  static_assert(std::is_same_v<T, yat::type_identity_t<T>>);

  if constexpr (!std::is_function_v<T>) {
    static_assert(
        std::is_same_v<const T, typename yat::type_identity<const T>::type>);
    static_assert(
        std::is_same_v<volatile T,
                       typename yat::type_identity<volatile T>::type>);
    static_assert(
        std::is_same_v<const volatile T,
                       typename yat::type_identity<const volatile T>::type>);

    static_assert(std::is_same_v<const T, yat::type_identity_t<const T>>);
    static_assert(std::is_same_v<volatile T, yat::type_identity_t<volatile T>>);
    static_assert(std::is_same_v<const volatile T,
                                 yat::type_identity_t<const volatile T>>);
  }

  if constexpr (!std::is_void_v<T>) {
    static_assert(std::is_same_v<T &, typename yat::type_identity<T &>::type>);
    static_assert(
        std::is_same_v<T &&, typename yat::type_identity<T &&>::type>);

    static_assert(std::is_same_v<T &, yat::type_identity_t<T &>>);
    static_assert(std::is_same_v<T &&, yat::type_identity_t<T &&>>);
  }

  if constexpr (!std::is_void_v<T> && !std::is_function_v<T>) {
    static_assert(std::is_same_v<const T &,
                                 typename yat::type_identity<const T &>::type>);
    static_assert(
        std::is_same_v<volatile T &,
                       typename yat::type_identity<volatile T &>::type>);
    static_assert(
        std::is_same_v<const volatile T &,
                       typename yat::type_identity<const volatile T &>::type>);
    static_assert(
        std::is_same_v<const T &&,
                       typename yat::type_identity<const T &&>::type>);
    static_assert(
        std::is_same_v<volatile T &&,
                       typename yat::type_identity<volatile T &&>::type>);
    static_assert(
        std::is_same_v<const volatile T &&,
                       typename yat::type_identity<const volatile T &&>::type>);

    static_assert(std::is_same_v<const T &, yat::type_identity_t<const T &>>);
    static_assert(
        std::is_same_v<volatile T &, yat::type_identity_t<volatile T &>>);
    static_assert(std::is_same_v<const volatile T &,
                                 yat::type_identity_t<const volatile T &>>);
    static_assert(std::is_same_v<const T &&, yat::type_identity_t<const T &&>>);
    static_assert(
        std::is_same_v<volatile T &&, yat::type_identity_t<volatile T &&>>);
    static_assert(std::is_same_v<const volatile T &&,
                                 yat::type_identity_t<const volatile T &&>>);
  }

  return true;
}

class C {};

TEST_CASE("type_identity", "[type_traits][type_identity]") {
  STATIC_REQUIRE(test_type_identity<void>());
  STATIC_REQUIRE(test_type_identity<int>());
  STATIC_REQUIRE(test_type_identity<int *>());
  STATIC_REQUIRE(test_type_identity<const int *>());
  STATIC_REQUIRE(test_type_identity<volatile int *>());
  STATIC_REQUIRE(test_type_identity<const volatile int *>());
  STATIC_REQUIRE(test_type_identity<int[3]>());
  STATIC_REQUIRE(test_type_identity<int[]>());
  STATIC_REQUIRE(test_type_identity<int(int)>());
  STATIC_REQUIRE(test_type_identity<int &(int)>());
  STATIC_REQUIRE(test_type_identity<const int &(int)>());
  STATIC_REQUIRE(test_type_identity<volatile int &(int)>());
  STATIC_REQUIRE(test_type_identity<const volatile int &(int)>());
  STATIC_REQUIRE(test_type_identity<int(int &)>());
  STATIC_REQUIRE(test_type_identity<int(const int &)>());
  STATIC_REQUIRE(test_type_identity<int(volatile int &)>());
  STATIC_REQUIRE(test_type_identity<int(const volatile int &)>());
  STATIC_REQUIRE(test_type_identity<int C::*>());
  STATIC_REQUIRE(test_type_identity<const int C::*>());
  STATIC_REQUIRE(test_type_identity<volatile int C::*>());
  STATIC_REQUIRE(test_type_identity<const volatile int C::*>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(int)>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(int &)>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(const int &) const>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(volatile int &) volatile>());
  STATIC_REQUIRE(
      test_type_identity<int (C::*)(const volatile int &) const volatile>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(int) &>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(int) const &>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(int) &&>());
  STATIC_REQUIRE(test_type_identity<int (C::*)(int) const &&>());
  STATIC_REQUIRE(test_type_identity<int &(C::*)(int)>());
  STATIC_REQUIRE(test_type_identity<const int &(C::*)(int)>());
  STATIC_REQUIRE(test_type_identity<volatile int &(C::*)(int)>());
  STATIC_REQUIRE(test_type_identity<const volatile int &(C::*)(int)>());
}

TEST_CASE("is_scoped_enum", "[type_traits][is_scoped_enum]") {
  enum A {};
  enum class B {};
  struct C {};

  STATIC_REQUIRE_FALSE(yat::is_scoped_enum_v<A>);
  STATIC_REQUIRE(yat::is_scoped_enum_v<B>);
  STATIC_REQUIRE_FALSE(yat::is_scoped_enum_v<C>);
  STATIC_REQUIRE_FALSE(yat::is_scoped_enum_v<int>);
}

template <typename T>
constexpr bool test_remove_cvref() {
  static_assert(std::is_same_v<T, typename yat::remove_cvref<T>::type>);
  static_assert(std::is_same_v<T, yat::remove_cvref_t<T>>);

  if constexpr (!std::is_function_v<T>) {
    static_assert(std::is_same_v<T, typename yat::remove_cvref<const T>::type>);
    static_assert(
        std::is_same_v<T, typename yat::remove_cvref<volatile T>::type>);
    static_assert(
        std::is_same_v<T, typename yat::remove_cvref<const volatile T>::type>);

    static_assert(std::is_same_v<T, yat::remove_cvref_t<const T>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<volatile T>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<const volatile T>>);
  }

  if constexpr (!std::is_void_v<T>) {
    static_assert(std::is_same_v<T, typename yat::remove_cvref<T &>::type>);
    static_assert(std::is_same_v<T, typename yat::remove_cvref<T &&>::type>);

    static_assert(std::is_same_v<T, yat::remove_cvref_t<T &>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<T &&>>);
  }

  if constexpr (!std::is_void_v<T> && !std::is_function_v<T>) {
    static_assert(
        std::is_same_v<T, typename yat::remove_cvref<const T &>::type>);
    static_assert(
        std::is_same_v<T, typename yat::remove_cvref<volatile T &>::type>);
    static_assert(
        std::is_same_v<T,
                       typename yat::remove_cvref<const volatile T &>::type>);
    static_assert(
        std::is_same_v<T, typename yat::remove_cvref<const T &&>::type>);
    static_assert(
        std::is_same_v<T, typename yat::remove_cvref<volatile T &&>::type>);
    static_assert(
        std::is_same_v<T,
                       typename yat::remove_cvref<const volatile T &&>::type>);

    static_assert(std::is_same_v<T, yat::remove_cvref_t<const T &>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<volatile T &>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<const volatile T &>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<const T &&>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<volatile T &&>>);
    static_assert(std::is_same_v<T, yat::remove_cvref_t<const volatile T &&>>);
  }

  return true;
}

TEST_CASE("remove_cvref", "[type_traits][remove_cvref]") {
  STATIC_REQUIRE(test_remove_cvref<void>());
  STATIC_REQUIRE(test_remove_cvref<int>());
  STATIC_REQUIRE(test_remove_cvref<int *>());
  STATIC_REQUIRE(test_remove_cvref<const int *>());
  STATIC_REQUIRE(test_remove_cvref<volatile int *>());
  STATIC_REQUIRE(test_remove_cvref<const volatile int *>());
  STATIC_REQUIRE(test_remove_cvref<int[3]>());
  STATIC_REQUIRE(test_remove_cvref<int[]>());
  STATIC_REQUIRE(test_remove_cvref<int(int)>());
  STATIC_REQUIRE(test_remove_cvref<int &(int)>());
  STATIC_REQUIRE(test_remove_cvref<const int &(int)>());
  STATIC_REQUIRE(test_remove_cvref<volatile int &(int)>());
  STATIC_REQUIRE(test_remove_cvref<const volatile int &(int)>());
  STATIC_REQUIRE(test_remove_cvref<int(int &)>());
  STATIC_REQUIRE(test_remove_cvref<int(const int &)>());
  STATIC_REQUIRE(test_remove_cvref<int(volatile int &)>());
  STATIC_REQUIRE(test_remove_cvref<int(const volatile int &)>());
  STATIC_REQUIRE(test_remove_cvref<int C::*>());
  STATIC_REQUIRE(test_remove_cvref<const int C::*>());
  STATIC_REQUIRE(test_remove_cvref<volatile int C::*>());
  STATIC_REQUIRE(test_remove_cvref<const volatile int C::*>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(int)>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(int &)>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(const int &) const>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(volatile int &) volatile>());
  STATIC_REQUIRE(
      test_remove_cvref<int (C::*)(const volatile int &) const volatile>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(int) &>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(int) const &>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(int) &&>());
  STATIC_REQUIRE(test_remove_cvref<int (C::*)(int) const &&>());
  STATIC_REQUIRE(test_remove_cvref<int &(C::*)(int)>());
  STATIC_REQUIRE(test_remove_cvref<const int &(C::*)(int)>());
  STATIC_REQUIRE(test_remove_cvref<volatile int &(C::*)(int)>());
  STATIC_REQUIRE(test_remove_cvref<const volatile int &(C::*)(int)>());
}

template <typename T, typename... Args>
struct TT {};

template <typename T>
struct T2 {};

TEST_CASE("is_specialization_of", "[type_traits][is_specialization_of]") {
  STATIC_REQUIRE(yat::is_specialization_of<TT<int>, TT>::value);
  STATIC_REQUIRE(yat::is_specialization_of_v<TT<int>, TT>);
  STATIC_REQUIRE(
      std::is_same_v<yat::is_specialization_of_t<TT<int>, TT>, std::true_type>);

  STATIC_REQUIRE(yat::is_specialization_of_v<TT<int, TT<int>>, TT>);
  STATIC_REQUIRE_FALSE(yat::is_specialization_of_v<TT<int>, T2>);
}

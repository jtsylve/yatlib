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

// Test the P0898R3 changes to <type_traits>:
// * Addition of yat::common_reference and yat::basic_common_reference
// * New fallback case for common_type

namespace {
template <class, class = void>
constexpr bool is_trait = false;
template <class T>
constexpr bool is_trait<T, std::void_t<typename T::type>> = true;

namespace detail {
static constexpr bool permissive() { return false; }

template <class>
struct DependentBase {
  static constexpr bool permissive() { return true; }
};

template <class T>
struct Derived : DependentBase<T> {
  static constexpr bool test() { return permissive(); }
};
}  // namespace detail
constexpr bool is_permissive = detail::Derived<int>::test();

struct move_only {
  move_only() = default;
  move_only(move_only &&) = default;
  move_only &operator=(move_only &&) = default;
};

template <class T>
struct converts_from {
  converts_from() = default;
  constexpr converts_from(T) noexcept {}
};

template <int>
struct interconvertible {
  interconvertible() = default;
  template <int N>
  explicit interconvertible(interconvertible<N>) {}
};

template <class... Ts>
struct derives_from : Ts... {};

struct simple_base {};
using simple_derived = derives_from<simple_base>;
}  // unnamed namespace

template <>
struct std::common_type<interconvertible<0>, interconvertible<1>> {
  using type = interconvertible<2>;
};
template <>
struct std::common_type<interconvertible<1>, interconvertible<0>> {
  using type = interconvertible<2>;
};

// A slightly simplified variation of std::tuple
template <class...>
struct tuple_ish {};

template <class, class, class>
struct tuple_ish_helper {};
template <class... Ts, class... Us>
struct tuple_ish_helper<std::void_t<yat::common_reference_t<Ts, Us>...>,
                        tuple_ish<Ts...>, tuple_ish<Us...>> {
  using type = tuple_ish<yat::common_reference_t<Ts, Us>...>;
};

template <class... Ts, class... Us, template <class> class TQual,
          template <class> class UQual>
struct yat::basic_common_reference<tuple_ish<Ts...>, tuple_ish<Us...>, TQual,
                                   UQual>
    : tuple_ish_helper<void, tuple_ish<TQual<Ts>...>, tuple_ish<UQual<Us>...>> {
};

// N4810 [meta.trans.other]/5.1: If sizeof...(T) is zero, there shall be no
// member type.
static_assert(!is_trait<yat::common_reference<>>);

// N4810 [meta.trans.other]/5.2: Otherwise, if sizeof...(T) is one, let T0
// denote the sole type in the pack T. The member typedef type shall denote the
// same type as T0.
static_assert(std::is_same_v<yat::common_reference_t<void>, void>);
static_assert(std::is_same_v<yat::common_reference_t<int>, int>);
static_assert(std::is_same_v<yat::common_reference_t<int &>, int &>);
static_assert(std::is_same_v<yat::common_reference_t<int &&>, int &&>);
static_assert(std::is_same_v<yat::common_reference_t<int const>, int const>);
static_assert(
    std::is_same_v<yat::common_reference_t<int const &>, int const &>);
static_assert(
    std::is_same_v<yat::common_reference_t<int const &&>, int const &&>);
static_assert(
    std::is_same_v<yat::common_reference_t<int volatile[]>, int volatile[]>);
static_assert(std::is_same_v<yat::common_reference_t<int volatile (&)[]>,
                             int volatile (&)[]>);
static_assert(std::is_same_v<yat::common_reference_t<int volatile(&&)[]>,
                             int volatile(&&)[]>);
static_assert(std::is_same_v<yat::common_reference_t<void (&)()>, void (&)()>);
static_assert(std::is_same_v<yat::common_reference_t<void(&&)()>, void(&&)()>);
static_assert(
    std::is_same_v<yat::common_reference_t<void() volatile>, void() volatile>);
static_assert(std::is_same_v<yat::common_reference_t<void() &&>, void() &&>);

// N4810 [meta.trans.other]/5.3.1: If T1 and T2 are reference types and
// COMMON_REF(T1, T2) is well-formed, then the member typedef type denotes that
// type.
static_assert(
    std::is_same_v<yat::common_reference_t<simple_base &, simple_derived &>,
                   simple_base &>);
static_assert(std::is_same_v<
              yat::common_reference_t<simple_base &, simple_derived const &>,
              simple_base const &>);
static_assert(
    std::is_same_v<yat::common_reference_t<simple_base &, simple_derived &&>,
                   simple_base const &>);
static_assert(std::is_same_v<
              yat::common_reference_t<simple_base &, simple_derived const &&>,
              simple_base const &>);

static_assert(std::is_same_v<
              yat::common_reference_t<simple_base const &, simple_derived &>,
              simple_base const &>);
static_assert(std::is_same_v<yat::common_reference_t<simple_base const &,
                                                     simple_derived const &>,
                             simple_base const &>);
static_assert(std::is_same_v<
              yat::common_reference_t<simple_base const &, simple_derived &&>,
              simple_base const &>);
static_assert(std::is_same_v<yat::common_reference_t<simple_base const &,
                                                     simple_derived const &&>,
                             simple_base const &>);

static_assert(
    std::is_same_v<yat::common_reference_t<simple_base &&, simple_derived &>,
                   simple_base const &>);
static_assert(std::is_same_v<
              yat::common_reference_t<simple_base &&, simple_derived const &>,
              simple_base const &>);
static_assert(
    std::is_same_v<yat::common_reference_t<simple_base &&, simple_derived &&>,
                   simple_base &&>);
static_assert(std::is_same_v<
              yat::common_reference_t<simple_base &&, simple_derived const &&>,
              simple_base const &&>);

static_assert(std::is_same_v<
              yat::common_reference_t<simple_base const &&, simple_derived &>,
              simple_base const &>);
static_assert(std::is_same_v<yat::common_reference_t<simple_base const &&,
                                                     simple_derived const &>,
                             simple_base const &>);
static_assert(std::is_same_v<
              yat::common_reference_t<simple_base const &&, simple_derived &&>,
              simple_base const &&>);
static_assert(std::is_same_v<yat::common_reference_t<simple_base const &&,
                                                     simple_derived const &&>,
                             simple_base const &&>);

#ifdef __EDG__
// When f is the name of a function of type int(), C1XX incorrectly believes
// that
//   decltype(false ? f : f)
// is int() in permissive mode and int(*)() in strict mode (Yes, two different
// incorrect results). It also correctly believes that
//   decltype(false ? declval<decltype((f))>() : declval<decltype((f))>())
// is int(&)(), which is nice because it allows this test case to pass. EDG
// believes the type of both the above is int() in all modes. I suspect this is
// intentional bug compatibility with C1XX, so I'm not filing a bug. I _do_
// assert here that EDG produces the _wrong_ type from yat::common_reference_t,
// however, so that THIS TEST WILL FAIL IF AND WHEN EDG STARTS BEHAVING
// CORRECTLY. We can then remove the non-workaround to defend against
// regression.
static_assert(
    !std::is_same_v<yat::common_reference_t<int (&)(), int (&)()>, int (&)()>);
static_assert(
    !std::is_same_v<yat::common_reference_t<int(&&)(), int (&)()>, int (&)()>);
static_assert(
    !std::is_same_v<yat::common_reference_t<int (&)(), int(&&)()>, int (&)()>);
static_assert(
    !std::is_same_v<yat::common_reference_t<int(&&)(), int(&&)()>, int(&&)()>);
#else   // ^^^ EDG / not EDG vvv
static_assert(
    std::is_same_v<yat::common_reference_t<int (&)(), int (&)()>, int (&)()>);
static_assert(
    std::is_same_v<yat::common_reference_t<int(&&)(), int (&)()>, int (&)()>);
static_assert(
    std::is_same_v<yat::common_reference_t<int (&)(), int(&&)()>, int (&)()>);
static_assert(
    std::is_same_v<yat::common_reference_t<int(&&)(), int(&&)()>, int(&&)()>);
#endif  // __EDG__

static_assert(std::is_same_v<
              yat::common_reference_t<int const volatile &&, int volatile &&>,
              int const volatile &&>);
static_assert(
    std::is_same_v<yat::common_reference_t<int &&, int const &, int volatile &>,
                   int const volatile &>);

template <class T = int>
constexpr bool strict_only_common_reference_cases() {
  if constexpr (!is_permissive) {
    static_assert(std::is_same_v<yat::common_reference_t<T(&)[10], T(&&)[10]>,
                                 T const(&)[10]>);
    static_assert(std::is_same_v<
                  yat::common_reference_t<T const(&)[10], T volatile(&)[10]>,
                  T const volatile(&)[10]>);
  }

  return true;
}
static_assert(strict_only_common_reference_cases());

// N4810 [meta.trans.other]/5.3.2: Otherwise, if
// yat::basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>, XREF(T1),
// XREF(T2)>::type is well-formed, then the member typedef type denotes that
// type.
static_assert(
    std::is_same_v<yat::common_reference_t<tuple_ish<int, short> const &,
                                           tuple_ish<int &, short volatile &>>,
                   tuple_ish<int const &, short const volatile &>>);
static_assert(
    std::is_same_v<yat::common_reference_t<tuple_ish<int, short> volatile &,
                                           tuple_ish<int, short> const &>,
                   tuple_ish<int, short> const volatile &>);

// N4810 [meta.trans.other]/5.3.3: Otherwise, if COND_RES(T1, T2) is
// well-formed, then the member typedef type denotes that type.
static_assert(std::is_same_v<yat::common_reference_t<void, void>, void>);
static_assert(std::is_same_v<yat::common_reference_t<void const, void>, void>);
static_assert(
    std::is_same_v<yat::common_reference_t<void volatile, void>, void>);
static_assert(
    std::is_same_v<yat::common_reference_t<void const volatile, void>, void>);
static_assert(std::is_same_v<yat::common_reference_t<int, short>, int>);
static_assert(std::is_same_v<yat::common_reference_t<int, short &>, int>);
static_assert(std::is_same_v<yat::common_reference_t<int &, short &>, int>);
static_assert(std::is_same_v<yat::common_reference_t<int &, short>, int>);

// tricky reference-to-volatile case
static_assert(
    std::is_same_v<yat::common_reference_t<int &&, int volatile &>, int>);
static_assert(
    std::is_same_v<yat::common_reference_t<int volatile &, int &&>, int>);

static_assert(
    std::is_same_v<yat::common_reference_t<int (&)[10], int (&)[11]>, int *>);

// https://github.com/ericniebler/stl2/issues/338
static_assert(
    std::is_same_v<yat::common_reference_t<int &, converts_from<int &>>,
                   converts_from<int &>>);

// N4810 [meta.trans.other]/5.3.4: Otherwise, if std::common_type_t<T1, T2> is
// well-formed, then the member typedef type denotes that type.
static_assert(
    std::is_same_v<yat::common_reference_t<interconvertible<0> &,
                                           interconvertible<1> const &>,
                   interconvertible<2>>);

static_assert(
    std::is_same_v<yat::common_reference_t<move_only const &, move_only>,
                   move_only>);
static_assert(
    std::is_same_v<
        yat::common_reference_t<derives_from<move_only> const &, move_only>,
        move_only>);
static_assert(std::is_same_v<yat::common_reference_t<move_only const &,
                                                     derives_from<move_only>>,
                             move_only>);

// N4810 [meta.trans.other]/5.3.5: Otherwise, there shall be no member type.
static_assert(!is_trait<yat::common_reference<tuple_ish<short> volatile &,
                                              tuple_ish<int, short> const &>>);

static_assert(
    !is_trait<yat::common_reference<void() volatile, void() volatile>>);
static_assert(!is_trait<yat::common_reference<void() volatile, int>>);
static_assert(!is_trait<yat::common_reference<int, void() volatile>>);
static_assert(!is_trait<yat::common_reference<void() &&, void() &&>>);
static_assert(!is_trait<yat::common_reference<int *, void() &&>>);
static_assert(!is_trait<yat::common_reference<void() &&, int (&)()>>);
static_assert(!is_trait<yat::common_reference<void() volatile, void() &&>>);

// N4810 [meta.trans.other]/5.4: Otherwise, if sizeof...(T) is greater than two,
// let T1, T2, and Rest, respectively, denote the first, second, and (pack of)
// remaining types comprising T. Let C be the type yat::common_reference_t<T1,
// T2>. Then: N4810 [meta.trans.other]/5.4.1: If there is such a type C, the
// member typedef type shall denote the same type, if any, as
// yat::common_reference_t<C, Rest...>.
static_assert(std::is_same_v<yat::common_reference_t<int, int, int>, int>);
static_assert(
    std::is_same_v<yat::common_reference_t<int &&, int const &, int volatile &>,
                   int const volatile &>);
static_assert(std::is_same_v<
              yat::common_reference_t<int &&, int const &, float &>, float>);

static_assert(
    std::is_same_v<yat::common_reference_t<
                       simple_base &, simple_derived const &, simple_derived &>,
                   simple_base const &>);
static_assert(
    std::is_same_v<yat::common_reference_t<simple_base &, simple_derived &,
                                           simple_base &, simple_derived &>,
                   simple_base &>);

// N4810 [meta.trans.other]/5.4.2: Otherwise, there shall be no member type.
static_assert(!is_trait<yat::common_reference<int, short, int, char *>>);

template <class T>
struct bad_reference_wrapper {
  bad_reference_wrapper(T &);
  bad_reference_wrapper(T &&) = delete;
  operator T &() const;
};

// N4810 [meta.trans.other]/3.3.4 (per the proposed resolution of LWG-3205):
// Otherwise, if
//   remove_cvref_t<decltype(false ? declval<const D1&>() : declval<const
//   D2&>())>
// denotes a type, let C denote that type.
static_assert(
    std::is_same_v<std::common_type_t<int, bad_reference_wrapper<int>>, int>);
static_assert(
    std::is_same_v<std::common_type_t<bad_reference_wrapper<double>, double>,
                   double>);
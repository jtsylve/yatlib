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

#include <catch2/catch.hpp>
#include <yatlib/bit.hpp>

using namespace yat;

// Tests adapted from
// https://github.com/microsoft/STL/blob/master/tests/std/tests/P0476R2_bit_cast/test.cpp

// structure sizing depends on a packing of 8
#pragma pack(8)

union test_union_1 {
  char a;
  int b;
};

union test_union_2 {
  char a;
  float b;
};

struct middle_class_1 {
  double d;
  virtual ~middle_class_1() = default;
  virtual void a_member_function_1() {}
};

struct middle_class_2 {
  int a;
  virtual ~middle_class_2() = default;
  virtual void a_member_function_2() {}
};

struct derived_class : middle_class_1, middle_class_2 {
  virtual void a_member_function_2() override {}
};

struct test_struct_1 {
  char a;
  // char[3]
  int b;
  short c;
  // char[6]
  double d;

  void a_member_function() {}
};

struct test_struct_2 {
  short a;
  // char[2]
  float b;
  float c;
  int d;
  unsigned int e;
  char f;
  // char[1]
  short g;

  void a_member_function() {}
};

struct test_struct_3_member_fn_pointer {
  char a;
  // char[3]
  int b;
  short c;
  char d[2];
  void (test_struct_1::*fn)();
};

struct test_struct_4_large_member_fn_pointer {
  char a;
  // char[3]
  int b;
  double c;
  double d;
  void (derived_class::*fn)();  // "large" member fn pointers are aligned to 8
                                // on both x64 and x86
};

struct test_struct_5_struct {
  char a;
  // char[3]
  int b;
  double c;
  double d;
  void* e;
  size_t f;
};

struct test_struct_6 {
  char a;
  // char[3]
  int b;
  int c;
  // char[4] on x64
  void* v;
};

struct test_struct_7_member_fn_pointer {
  char a;
  // char[3]
  int b;
  int c;
  void (test_struct_1::*fn)();
};

struct test_struct_1_not_trivially_copyable {
  char a;
  // char[3]
  int b;
  short c;
  // char[2]
  double d;

  test_struct_1_not_trivially_copyable& operator=(
      const test_struct_1_not_trivially_copyable&) {
    return *this;
  }
};

#pragma pack(push, 1)
struct test_struct_1_packed {
  char a;
  int b;
  short c;
  double d;
};
struct test_struct_2_packed {
  short a;
  float b;
  float c;
  int d;
  unsigned int e;
  char f;
  short g;
};
#pragma pack(pop)

template <typename To, typename From, typename = void>
constexpr bool bit_cast_invocable = false;

template <typename To, typename From>
constexpr bool bit_cast_invocable<
    To, From, std::void_t<decltype(bit_cast<To>(std::declval<From>()))>> = true;

template <int zero = 0,
          int = ((void)bit_cast<test_union_1>(test_union_2{}), zero)>
constexpr bool bit_cast_is_constexpr_union(int) {
  return true;
}

constexpr bool bit_cast_is_constexpr_union(long) { return false; }

template <int zero = 0, int = ((void)bit_cast<float*>(nullptr), zero)>
constexpr bool bit_cast_is_constexpr_pointer(int) {
  return true;
}

constexpr bool bit_cast_is_constexpr_pointer(long) { return false; }

template <int zero = 0, int = ((void)bit_cast<void (test_struct_1::*)()>(
                                   &test_struct_2::a_member_function),
                               zero)>
constexpr bool bit_cast_is_constexpr_member_fn_pointer(int) {
  return true;
}

constexpr bool bit_cast_is_constexpr_member_fn_pointer(long) { return false; }

constexpr bool bit_cast_is_constexpr_pmf_datamember(long) { return false; }

template <int zero = 0, int = ((void)bit_cast<test_struct_5_struct>(
                                   test_struct_4_large_member_fn_pointer{}),
                               zero)>
constexpr bool bit_cast_is_constexpr_large_member_fn_pointer(int) {
  return true;
}

constexpr bool bit_cast_is_constexpr_large_member_fn_pointer(long) {
  return false;
}

template <typename To, typename From>
void zero_initialized_round_trip() {
  From before{};
  To middle = bit_cast<To>(before);
  REQUIRE(memcmp(&before, &middle, sizeof(From)) == 0);
  From after = bit_cast<From>(middle);
  REQUIRE(memcmp(&before, &after, sizeof(From)) == 0);
}

TEST_CASE("bit_cast", "[bit][bit_cast]") {
  STATIC_REQUIRE_FALSE(
      bit_cast_invocable<test_struct_2_packed, test_struct_1_packed>);
  STATIC_REQUIRE_FALSE(
      bit_cast_invocable<test_struct_1_not_trivially_copyable, test_struct_1>);
  STATIC_REQUIRE_FALSE(
      bit_cast_invocable<test_struct_1, test_struct_1_not_trivially_copyable>);
  STATIC_REQUIRE_FALSE(
      bit_cast_invocable<test_struct_1_not_trivially_copyable,
                         test_struct_1_not_trivially_copyable>);
  STATIC_REQUIRE(bit_cast_invocable<test_union_1, test_union_2>);

  // TODO(JTS): Why does this fail?
  // STATIC_REQUIRE(bit_cast_invocable<ptrdiff_t, void (test_struct_1::*)()>);

  STATIC_REQUIRE(bit_cast_invocable<ptrdiff_t, void (*)()>);
  STATIC_REQUIRE(bit_cast_invocable<test_struct_1, test_struct_2>);
  STATIC_REQUIRE(bit_cast_invocable<test_struct_4_large_member_fn_pointer,
                                    test_struct_5_struct>);

  // Tests for conditions on constexprness
  STATIC_REQUIRE_FALSE(bit_cast_is_constexpr_union(0));
  STATIC_REQUIRE_FALSE(bit_cast_is_constexpr_pointer(0));
  STATIC_REQUIRE_FALSE(bit_cast_is_constexpr_member_fn_pointer(0));
  STATIC_REQUIRE_FALSE(bit_cast_is_constexpr_pmf_datamember(0));
  STATIC_REQUIRE_FALSE(bit_cast_is_constexpr_large_member_fn_pointer(0));

  zero_initialized_round_trip<test_struct_1, test_struct_2>();
  zero_initialized_round_trip<test_struct_3_member_fn_pointer,
                              test_struct_3_member_fn_pointer>();
  zero_initialized_round_trip<test_struct_3_member_fn_pointer,
                              test_struct_7_member_fn_pointer>();
  zero_initialized_round_trip<test_struct_4_large_member_fn_pointer,
                              test_struct_5_struct>();

  zero_initialized_round_trip<float, int>();
  zero_initialized_round_trip<double, long long>();
  zero_initialized_round_trip<unsigned int, float>();
}

TEST_CASE("bit_cast floats", "[bit_cast][bit]") {
  unsigned int as_int = bit_cast<unsigned int>(0x0.000002p-126f);
  REQUIRE(as_int == 1);
  REQUIRE(bit_cast<float>(as_int) == 0x0.000002p-126f);
  as_int = bit_cast<unsigned int>(0x1.1p1f);
  REQUIRE(as_int == 0x40080000);
  REQUIRE(bit_cast<float>(as_int) == 0x1.1p1f);
  as_int = bit_cast<unsigned int>(0x0.0p0f);
  REQUIRE(as_int == 0);
  REQUIRE(bit_cast<float>(as_int) == 0x0.0p0f);

  REQUIRE(std::signbit(bit_cast<float>(as_int)) == false);

  as_int = bit_cast<unsigned int>(-0x0.0p0f);
  REQUIRE(as_int == 0x80000000);
  REQUIRE(bit_cast<float>(as_int) == -0x0.0p0f);
  REQUIRE(std::signbit(bit_cast<float>(as_int)) == true);

  // signaling nan
  as_int = 0x7fc00001;
  float snan = bit_cast<float>(as_int);
  REQUIRE(as_int == bit_cast<unsigned int>(snan));
  as_int = bit_cast<unsigned int>(std::numeric_limits<float>::infinity());
  REQUIRE(as_int == 0x7f800000);
  REQUIRE(bit_cast<float>(as_int) == std::numeric_limits<float>::infinity());
}

#ifdef YAT_HAS_CONSTEXPR_BIT_CAST
TEST_CASE("bit_cast floats (constexpr)", "[bit_cast][bit][constexpr]") {
  constexpr unsigned int as_int1 = bit_cast<unsigned int>(0x0.000002p-126f);
  STATIC_REQUIRE(as_int1 == 1);
  STATIC_REQUIRE(bit_cast<float>(as_int1) == 0x0.000002p-126f);

  constexpr unsigned int as_int2 = bit_cast<unsigned int>(0x1.1p1f);
  STATIC_REQUIRE(as_int2 == 0x40080000);
  STATIC_REQUIRE(bit_cast<float>(as_int2) == 0x1.1p1f);

  constexpr unsigned int as_int3 = bit_cast<unsigned int>(0x0.0p0f);
  STATIC_REQUIRE(as_int3 == 0);
  STATIC_REQUIRE(bit_cast<float>(as_int3) == 0x0.0p0f);

  constexpr unsigned int as_int4 = bit_cast<unsigned int>(-0x0.0p0f);
  STATIC_REQUIRE(as_int4 == 0x80000000);
  STATIC_REQUIRE(bit_cast<float>(as_int4) == -0x0.0p0f);

  // signaling nan
  constexpr unsigned int as_int5 = 0x7fc00001;
  constexpr float snan = bit_cast<float>(as_int5);
  STATIC_REQUIRE(as_int5 == bit_cast<unsigned int>(snan));

  constexpr unsigned int as_int6 =
      bit_cast<unsigned int>(std::numeric_limits<float>::infinity());
  STATIC_REQUIRE(as_int6 == 0x7f800000);
  STATIC_REQUIRE(bit_cast<float>(as_int6) ==
                 std::numeric_limits<float>::infinity());
}
#endif  // YAT_HAS_CONSTEXPR_BIT_CAST

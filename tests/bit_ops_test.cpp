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

#include <limits>
#include <yatlib/bit.hpp>

#include "common.hpp"

// Tests adapted from
// https://github.com/microsoft/STL/blob/master/tests/std/tests/P0553R4_bit_rotating_and_counting_functions/test.cpp

template <typename T>
static void test_countl_zero() {
  constexpr int digits = std::numeric_limits<T>::digits;
  CHECK(yat::countl_zero(T{0}) == digits);
  CHECK(yat::countl_zero(std::numeric_limits<T>::max()) == 0);
  CHECK(yat::countl_zero(T{1}) == digits - 1);
  for (int i = 1; i < digits; ++i) {
    const auto one_in_ith_place = static_cast<T>(T{1} << i);
    CHECK(yat::countl_zero(one_in_ith_place) == digits - i - 1);
    CHECK(yat::countl_zero(static_cast<T>(one_in_ith_place - 1)) == digits - i);
    CHECK(yat::countl_zero(static_cast<T>(one_in_ith_place + 1)) ==
          digits - i - 1);
  }
}

template <typename T>
static void test_countr_zero() {
  constexpr int digits = std::numeric_limits<T>::digits;
  CHECK(yat::countr_zero(T{0}) == digits);
  CHECK(yat::countr_zero(std::numeric_limits<T>::max()) == 0);
  CHECK(yat::countr_zero(T{1}) == 0);
  for (int i = 1; i < digits; ++i) {
    const auto one_in_ith_place = static_cast<T>(T{1} << i);
    CHECK(yat::countr_zero(one_in_ith_place) == i);
    CHECK(yat::countr_zero(static_cast<T>(one_in_ith_place + 1)) == 0);
    CHECK(yat::countr_zero(static_cast<T>(one_in_ith_place - 1)) == 0);
  }
}

template <typename T>
static void test_countl_one() {
  constexpr int digits = std::numeric_limits<T>::digits;
  CHECK(yat::countl_one(std::numeric_limits<T>::max()) == digits);
  CHECK(yat::countl_one(T{0}) == 0);
  CHECK(yat::countl_one(static_cast<T>(~T{1})) == digits - 1);
  CHECK(yat::countl_one(static_cast<T>(~T{1} + 1)) == digits);
  for (int i = 1; i < digits; ++i) {
    const auto zero_in_ith_place = static_cast<T>(~(T{1} << i));
    CHECK(yat::countl_one(zero_in_ith_place) == digits - i - 1);
    CHECK(yat::countl_one(static_cast<T>(zero_in_ith_place + 1)) == digits - i);
    CHECK(yat::countl_one(static_cast<T>(zero_in_ith_place - 1)) ==
          digits - i - 1);
  }
}

template <typename T>
static void test_countr_one() {
  constexpr int digits = std::numeric_limits<T>::digits;
  CHECK(yat::countr_one(T{0}) == 0);
  CHECK(yat::countr_one(std::numeric_limits<T>::max()) == digits);
  CHECK(yat::countr_one(static_cast<T>(~T{1})) == 0);
  for (int i = 1; i < digits; ++i) {
    const auto zero_in_ith_place = static_cast<T>(~(T{1} << i));
    CHECK(yat::countr_one(zero_in_ith_place) == i);
    CHECK(yat::countr_one(static_cast<T>(zero_in_ith_place - 1)) == 0);
    CHECK(yat::countr_one(static_cast<T>(zero_in_ith_place + 1)) == 0);
  }
}

template <typename T>
static void test_popcount() {
  constexpr int digits = std::numeric_limits<T>::digits;
  CHECK(yat::popcount(T{0}) == 0);
  CHECK(yat::popcount(std::numeric_limits<T>::max()) == digits);
  CHECK(yat::popcount(T{1}) == 1);
  for (int i = 1; i < digits; ++i) {
    const auto one_in_ith_place = static_cast<T>(T{1} << i);
    CHECK(yat::popcount(one_in_ith_place) == 1);
    CHECK(yat::popcount(static_cast<T>(one_in_ith_place - 1)) == i);
    CHECK(yat::popcount(static_cast<T>(one_in_ith_place + 1)) == 2);
  }
}

template <typename T>
static void test_rotl() {
  constexpr int digits = std::numeric_limits<T>::digits;
  constexpr auto maxval = std::numeric_limits<T>::max();
  for (int i = 0; i < digits * 2; ++i) {
    CHECK(yat::rotl(maxval, i) == maxval);
    CHECK(yat::rotl(T{0}, i) == 0);
  }
}

template <typename T>
static void test_rotr() {
  constexpr int digits = std::numeric_limits<T>::digits;
  constexpr auto maxval = std::numeric_limits<T>::max();
  for (int i = 0; i < digits * 2; ++i) {
    CHECK(yat::rotr(maxval, i) == maxval);
    CHECK(yat::rotr(T{0}, i) == 0);
  }
}

// Tests functions for 64-bit operands that have either high or low halves as
// zero. These may be split into two operations on 32-bit platforms and we need
// to check if we handle the == zero or == ones case correctly.
static void test_64bit_split_ops() {
  constexpr unsigned long long zero_one = 0x0000'0000'FFFF'FFFF;
  constexpr unsigned long long one_zero = 0xFFFF'FFFF'0000'0000;
  CHECK(yat::popcount(zero_one) == 32);
  CHECK(yat::popcount(one_zero) == 32);
  CHECK(yat::countr_zero(zero_one) == 0);
  CHECK(yat::countr_zero(one_zero) == 32);
  CHECK(yat::countl_zero(zero_one) == 32);
  CHECK(yat::countl_zero(one_zero) == 0);
  CHECK(yat::countr_one(zero_one) == 32);
  CHECK(yat::countr_one(one_zero) == 0);
  CHECK(yat::countl_one(zero_one) == 0);
  CHECK(yat::countl_one(one_zero) == 32);
}

template <typename T>
static void test_popcount_specialcases() {
  constexpr int digits = std::numeric_limits<T>::digits;
  CHECK(yat::popcount(T{0x0}) == 0);
  CHECK(yat::popcount(T{0xFF}) == 8);
  CHECK(yat::popcount(T{0xAA}) == 4);
  CHECK(yat::popcount(T{0x80}) == 1);
  CHECK(yat::popcount(T{0x55}) == 4);

  if constexpr (digits >= 16) {
    CHECK(yat::popcount(T{0xFFFF}) == 16);
    CHECK(yat::popcount(T{0xAAAA}) == 8);
    CHECK(yat::popcount(T{0x8000}) == 1);
    CHECK(yat::popcount(T{0x5555}) == 8);
  }

  if constexpr (digits >= 32) {
    CHECK(yat::popcount(T{0xFFFFFFFF}) == 32);
    CHECK(yat::popcount(T{0xAAAAAAAA}) == 16);
    CHECK(yat::popcount(T{0x80000000}) == 1);
    CHECK(yat::popcount(T{0x55555555}) == 16);
  }

  if constexpr (digits >= 64) {
    CHECK(yat::popcount(T{0xFFFFFFFF'FFFFFFFF}) == 64);
    CHECK(yat::popcount(T{0xAAAAAAAA'AAAAAAAA}) == 32);
    CHECK(yat::popcount(T{0x80000000'00000000}) == 1);
    CHECK(yat::popcount(T{0x55555555'55555555}) == 32);
  }
}

template <typename T>
static void test_all() {
  test_countl_zero<T>();
  test_countr_zero<T>();
  test_countl_one<T>();
  test_countr_one<T>();
  test_popcount<T>();
  test_rotl<T>();
  test_rotr<T>();
  test_64bit_split_ops();
  test_popcount_specialcases<T>();
}

TEST_CASE("bit operations", "[bit][bit_ops]") {
  test_all<unsigned char>();
  test_all<unsigned short>();
  test_all<unsigned int>();
  test_all<unsigned long>();
  test_all<unsigned long long>();
}
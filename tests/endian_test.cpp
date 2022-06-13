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

#include <cstddef>
#include <cstring>
#include <random>
#include <vector>
#include <yatlib/endian.hpp>

#include "common.hpp"

// Inspired by
// https://github.com/abseil/abseil-cpp/blob/master/absl/base/internal/endian_test.cc

const int num_random_values = 100000;

template <typename T>
static void endian_test(const std::vector<T>& values_to_test) {
  for (const T& value : values_to_test) {
    const yat::little_scalar<T> little_val = value;
    const yat::big_scalar<T> big_val = value;

    // get the scalar swapper type
    const typename yat::little_scalar<T>::byte_swapper_type swapper{};

    REQUIRE(little_val == value);
    REQUIRE(big_val == value);
    REQUIRE(little_val == big_val);

    REQUIRE(yat::bit_cast<T>(little_val) == swapper(yat::bit_cast<T>(big_val)));
    REQUIRE(yat::bit_cast<T>(big_val) == swapper(yat::bit_cast<T>(little_val)));

    if constexpr (yat::is_little_endian_system) {
      REQUIRE(yat::bit_cast<T>(little_val) == value);
      REQUIRE(yat::bit_cast<T>(big_val) == swapper(value));
    }

    if constexpr (yat::is_big_endian_system) {
      REQUIRE(yat::bit_cast<T>(big_val) == value);
      REQUIRE(yat::bit_cast<T>(little_val) == swapper(value));
    }
  }
}

TEST_CASE("endian_scalar", "[endian][endian_scalar]") {
  STATIC_REQUIRE(std::is_standard_layout_v<yat::big_uint64_t>);

  // Test swapping all integers
  endian_test(generate_all_values<int8_t>());
  endian_test(generate_all_values<uint8_t>());
  endian_test(generate_all_values<int16_t>());
  endian_test(generate_all_values<uint16_t>());

  // There are too many of these so let's do some random sampling
  endian_test(generate_random_values<int32_t>(num_random_values));
  endian_test(generate_random_values<uint32_t>(num_random_values));
  endian_test(generate_random_values<int64_t>(num_random_values));
  endian_test(generate_random_values<uint64_t>(num_random_values));
}

struct S {
  constexpr S() noexcept = default;
  // cppcheck-suppress noExplicitConstructor
  constexpr S(uint64_t v) noexcept : value{v} {}
  constexpr operator uint64_t() const noexcept { return value; }

  uint64_t value{};
};

constexpr bool operator==(const S& lhs, const S& rhs) noexcept {
  return (lhs.value == rhs.value);
}

template <>
struct yat::endian_byte_swapper<S> {
  constexpr S operator()(const S& s) const noexcept {
    return yat::byteswap(s.value);
  }
};

TEST_CASE("endian_scalar (custom)", "[endian][endian_scalar]") {
  endian_test(generate_random_values<S>(num_random_values));
}

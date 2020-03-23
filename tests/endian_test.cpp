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

using namespace yat;

// Inspired by
// https://github.com/abseil/abseil-cpp/blob/master/absl/base/internal/endian_test.cc

const int num_random_values = 1000000;

template <typename T>
static void endian_test(const std::vector<T>& values_to_test) {
  for (const T value : values_to_test) {
    const little_scaler<T> little_val = value;
    const big_scaler<T> big_val = value;

    REQUIRE(little_val == value);
    REQUIRE(big_val == value);
    REQUIRE(little_val == big_val);

    REQUIRE(bit_cast<T>(little_val) == byteswap(bit_cast<T>(big_val)));
    REQUIRE(bit_cast<T>(big_val) == byteswap(bit_cast<T>(little_val)));

    if constexpr (is_little_endian_system) {
      REQUIRE(bit_cast<T>(little_val) == value);
      REQUIRE(bit_cast<T>(big_val) == byteswap(value));
    }

    if constexpr (is_big_endian_system) {
      REQUIRE(bit_cast<T>(big_val) == value);
      REQUIRE(bit_cast<T>(little_val) == byteswap(value));
    }
  }
}

TEST_CASE("endian_scalar", "[endian]") {
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
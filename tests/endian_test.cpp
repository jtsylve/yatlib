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

#include "catch_config.hpp"

using namespace yat;

// Inspired by
// https://github.com/abseil/abseil-cpp/blob/master/absl/base/internal/endian_test.cc

const int num_random_values = 1000000;
const int random_seed = 12345;

template <typename T>
static std::vector<T> generate_all_values() {
  std::vector<T> result;
  T next = std::numeric_limits<T>::min();
  while (true) {
    result.push_back(next);
    if (next == std::numeric_limits<T>::max()) {
      return result;
    }
    ++next;
  }
}

template <typename T>
static std::vector<T> generate_random_values(size_t numValuesToTest) {
  std::vector<T> result;
  std::mt19937_64 rng(random_seed);
  for (size_t i = 0; i < numValuesToTest; ++i) {
    result.push_back(static_cast<T>(rng()));
  }
  return result;
}

static void manual_byte_swap(std::byte* bytes, int length) {
  if (length == 1) {
    return;
  }

  REQUIRE(length % 2 == 0);

  for (int i = 0; i < length / 2; ++i) {
    int j = (length - 1) - i;
    using std::swap;
    swap(bytes[i], bytes[j]);
  }
}

template <typename T>
static void swap_test(const std::vector<T>& host_values_to_test) {
  for (const T value : host_values_to_test) {
    std::byte swapped_bytes[sizeof(T)];
    memcpy(swapped_bytes, &value, sizeof(T));
    manual_byte_swap(swapped_bytes, sizeof(T));

    REQUIRE(swap_endian(value) == bit_cast<T>(swapped_bytes));
  }
}

TEST_CASE("swap_endian", "[endian]") {
  // Test swapping all integers
  swap_test(generate_all_values<int8_t>());
  swap_test(generate_all_values<uint8_t>());
  swap_test(generate_all_values<int16_t>());
  swap_test(generate_all_values<uint16_t>());

  // There are too many of these so let's do some random sampling
  swap_test(generate_random_values<int32_t>(num_random_values));
  swap_test(generate_random_values<uint32_t>(num_random_values));
  swap_test(generate_random_values<int64_t>(num_random_values));
  swap_test(generate_random_values<uint64_t>(num_random_values));
}

template <typename T>
static void endian_test(const std::vector<T>& values_to_test) {
  for (const T value : values_to_test) {
    const le<T> little_val = value;
    const be<T> big_val = value;

    REQUIRE(little_val == value);
    REQUIRE(big_val == value);
    REQUIRE(little_val == big_val);

    REQUIRE(bit_cast<T>(little_val) == swap_endian(bit_cast<T>(big_val)));
    REQUIRE(bit_cast<T>(big_val) == swap_endian(bit_cast<T>(little_val)));

    if constexpr (is_little_endian_system) {
      REQUIRE(bit_cast<T>(little_val) == value);
      REQUIRE(bit_cast<T>(big_val) == swap_endian(value));
    }

    if constexpr (is_big_endian_system) {
      REQUIRE(bit_cast<T>(big_val) == value);
      REQUIRE(bit_cast<T>(little_val) == swap_endian(value));
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
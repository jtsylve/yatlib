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
#include <yatlib/bit.hpp>

#include "common.hpp"

constexpr int num_random_values = 1000000;

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
    std::memcpy(swapped_bytes, &value, sizeof(T));
    manual_byte_swap(swapped_bytes, sizeof(T));

    REQUIRE(yat::byteswap(value) == yat::bit_cast<T>(swapped_bytes));
  }
}

TEST_CASE("byteswap integral", "[bit][byteswap]") {
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

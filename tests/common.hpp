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

YAT_IGNORE_WARNING_PUSH(4619);
#include <catch2/catch.hpp>
YAT_IGNORE_WARNING_POP();

constexpr int random_seed = 12345;

template <typename T>
std::vector<T> generate_all_values() {
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
std::vector<T> generate_random_values(size_t numValuesToTest) {
  std::vector<T> result;
  std::mt19937_64 rng(random_seed);
  for (size_t i = 0; i < numValuesToTest; ++i) {
    result.push_back(static_cast<T>(rng()));
  }
  return result;
}

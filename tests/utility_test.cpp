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
#include <cstdint>
#include <yatlib/utility.hpp>

#include "common.hpp"

using namespace yat;

TEST_CASE("to_underlying", "[utlity][to_underlying]") {
  enum class unspecified_enum {};
  enum class int_enum : int {};
  enum class int8_t_enum : int8_t {};
  enum class uint8_t_enum : uint8_t {};
  enum class int16_t_enum : int16_t {};
  enum class uint16_t_enum : uint16_t {};
  enum class int32_t_enum : int32_t {};
  enum class uint32_t_enum : uint32_t {};
  enum class int64_t_enum : int64_t {};
  enum class uint64_t_enum : uint64_t {};

  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(unspecified_enum{})), int>);
  STATIC_REQUIRE(std::is_same_v<decltype(to_underlying(int_enum{})), int>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(int8_t_enum{})), int8_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(uint8_t_enum{})), uint8_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(int16_t_enum{})), int16_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(uint16_t_enum{})), uint16_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(int32_t_enum{})), int32_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(uint32_t_enum{})), uint32_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(int64_t_enum{})), int64_t>);
  STATIC_REQUIRE(
      std::is_same_v<decltype(to_underlying(uint64_t_enum{})), uint64_t>);
}
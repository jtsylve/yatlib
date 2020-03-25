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

#include <type_traits>
#include <yatlib/format.hpp>

#include "common.hpp"

using namespace yat;

static void format_parse_context_test() noexcept {
  // context should not be copyable
  STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<format_parse_context>);
  STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<format_parse_context>);

  constexpr std::string_view fmt1 = "Hello, {}!";

  auto context1 = basic_format_parse_context{fmt1};

  // cppcheck-suppress mismatchingContainerExpression
  REQUIRE(context1.begin() == fmt1.begin());

  // cppcheck-suppress mismatchingContainerExpression
  REQUIRE(context1.end() == fmt1.end());

  constexpr auto it1 = fmt1.begin() + 1;

  context1.advance_to(it1);
  REQUIRE(context1.begin() == it1);

  REQUIRE_NOTHROW(context1.next_arg_id() == 1);
  REQUIRE_THROWS_AS(context1.check_arg_id(0), format_error);

  auto context2 = basic_format_parse_context{fmt1};
  REQUIRE_NOTHROW(context2.check_arg_id(0));
  REQUIRE_THROWS_AS(context2.next_arg_id(), format_error);
}

TEST_CASE("format_parse_context", "[format][format_parse_context]") {
  format_parse_context_test();
}
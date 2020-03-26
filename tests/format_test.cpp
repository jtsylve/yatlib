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

TEST_CASE("formatter (disabled)", "[format][formatter]") {
  struct dummy {};

  // In disabled specialization of a formatter, F, these values are false:
  // is_default_constructible_v<F>, is_copy_constructible_v<F>,
  // is_move_constructible_v<F>, is_copy_assignable_v<F>,
  // is_move_assignable_v<F>
  STATIC_REQUIRE_FALSE(std::is_default_constructible_v<yat::formatter<dummy>>);
  STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<yat::formatter<dummy>>);
  STATIC_REQUIRE_FALSE(std::is_move_constructible_v<yat::formatter<dummy>>);
  STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<yat::formatter<dummy>>);
  STATIC_REQUIRE_FALSE(std::is_move_assignable_v<yat::formatter<dummy>>);
}

TEST_CASE("basic_format_parse_context",
          "[format][basic_format_parse_context]") {
  // context should not be copyable
  STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<yat::format_parse_context>);
  STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<yat::format_parse_context>);

  constexpr std::string_view fmt1 = "Hello, {}!";

  auto context1 = yat::basic_format_parse_context{fmt1};

  // cppcheck-suppress mismatchingContainerExpression
  REQUIRE(context1.begin() == fmt1.begin());

  // cppcheck-suppress mismatchingContainerExpression
  REQUIRE(context1.end() == fmt1.end());

  constexpr auto it1 = fmt1.begin() + 1;

  context1.advance_to(it1);
  REQUIRE(context1.begin() == it1);

  REQUIRE_NOTHROW(context1.next_arg_id() == 1);
  REQUIRE_THROWS_AS(context1.check_arg_id(0), yat::format_error);

  auto context2 = yat::basic_format_parse_context{fmt1};
  REQUIRE_NOTHROW(context2.check_arg_id(0));
  REQUIRE_THROWS_AS(context2.next_arg_id(), yat::format_error);
}

struct dummy_context {
  using char_type = char;

  template <typename T>
  using formatter_type = yat::formatter<T, char_type>;
};

TEST_CASE("basic_format_arg", "[format][basic_format_arg]") {
  yat::basic_format_arg<dummy_context> default_arg;
  REQUIRE(!default_arg);

  constexpr auto monostate_visitor = [](const auto& arg) {
    using arg_type = std::decay_t<decltype(arg)>;

    if constexpr (std::is_same_v<arg_type, std::monostate>) {
      return true;
    } else {
      return false;
    }
  };

  REQUIRE(yat::visit_format_arg(monostate_visitor, default_arg));
}

TEST_CASE("make_format_args", "[format][make_format_args]") {
  enum arg_t {
    unknown_type,
    int_type,
    float_type,
    unsigned_int_type,
  };

  constexpr auto visitor = [](const auto& arg) {
    using arg_type = std::decay_t<decltype(arg)>;

    if constexpr (std::is_same_v<arg_type, int>) {
      return int_type;
    } else if constexpr (std::is_same_v<arg_type, float>) {
      return float_type;
    } else if constexpr (std::is_same_v<arg_type, unsigned int>) {
      return unsigned_int_type;
    } else {
      return unknown_type;
    }
  };

  const auto args = yat::make_format_args<dummy_context>(1, 2.0f, 3U);

  REQUIRE(args.size() == 3);
  REQUIRE(yat::visit_format_arg(visitor, args[0]) == int_type);
  REQUIRE(yat::visit_format_arg(visitor, args[1]) == float_type);
  REQUIRE(yat::visit_format_arg(visitor, args[2]) == unsigned_int_type);
}

TEST_CASE("basic_format_args", "[format][basic_format_args]") {
  yat::basic_format_args<dummy_context> default_args;
  REQUIRE_FALSE(default_args.get(0));

  enum arg_t {
    unknown_type,
    int_type,
    float_type,
    unsigned_int_type,
  };

  constexpr auto visitor = [](const auto& arg) {
    using arg_type = std::decay_t<decltype(arg)>;

    if constexpr (std::is_same_v<arg_type, int>) {
      return int_type;
    } else if constexpr (std::is_same_v<arg_type, float>) {
      return float_type;
    } else if constexpr (std::is_same_v<arg_type, unsigned int>) {
      return unsigned_int_type;
    } else {
      return unknown_type;
    }
  };

  const auto store = yat::make_format_args<dummy_context>(1, 2.0f, 3U);
  yat::basic_format_args<dummy_context> args{store};
  REQUIRE_FALSE(args.get(3));

  REQUIRE(yat::visit_format_arg(visitor, args.get(0)) == int_type);
  REQUIRE(yat::visit_format_arg(visitor, args.get(1)) == float_type);
  REQUIRE(yat::visit_format_arg(visitor, args.get(2)) == unsigned_int_type);
}

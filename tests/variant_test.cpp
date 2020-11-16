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
#include <array>
#include <type_traits>
#include <yatlib/variant.hpp>

#include "common.hpp"

TEST_CASE("bad_variant_access", "[variant][bad_variant_access]") {
  REQUIRE(yat::bad_variant_access().what() != nullptr);

  constexpr auto do_throw = [] { throw yat::bad_variant_access(); };

  REQUIRE_THROWS_AS(do_throw(), yat::bad_variant_access);
}

TEST_CASE("visit", "[variant][visit]") {
  using variant_t = yat::variant<std::monostate, int>;

  variant_t v1 = 1;
  variant_t v2;

  constexpr auto visitor = [](const auto& val) {
    using type = std::decay_t<decltype(val)>;

    if constexpr (std::is_same_v<type, int>) {
      return true;
    } else {
      return false;
    }
  };

  REQUIRE(yat::visit(visitor, v1));
  REQUIRE_FALSE(yat::visit(visitor, v2));
}

TEST_CASE("overloaded", "[variant][overloaded]") {
  using variant_t = yat::variant<std::monostate, int, float, bool>;

  variant_t v1 = 1;
  variant_t v2 = 2.0f;
  variant_t v3;
  variant_t v4 = true;

  enum class RT {
    DEFAULT,
    INT,
    FLOAT,
  };

  const yat::overloaded overload{[](int) { return RT::INT; },
                                 [](float) { return RT::FLOAT; },
                                 [](auto) { return RT::DEFAULT; }};

  REQUIRE(yat::visit(overload, v1) == RT::INT);
  REQUIRE(yat::visit(overload, v2) == RT::FLOAT);
  REQUIRE(yat::visit(overload, v3) == RT::DEFAULT);
  REQUIRE(yat::visit(overload, v4) == RT::DEFAULT);
}

TEST_CASE("match", "[variant][match]") {
  using variant_t = yat::variant<std::monostate, int, float, bool>;

  const std::array<variant_t, 4> variants = {1, 2.0f, true};

  enum class RT {
    DEFAULT,
    INT,
    FLOAT,
  };

  for (size_t i = 0; i < variants.size(); ++i) {
    const auto expected = [i] {
      switch (i) {
        case 0:
          return RT::INT;
        case 1:
          return RT::FLOAT;
        default:
          return RT::DEFAULT;
      }
    }();

    const auto result = yat::match(
        variants[i], [](int) { return RT::INT; },
        [](float) { return RT::FLOAT; }, [](auto) { return RT::DEFAULT; });

    REQUIRE(expected == result);
  }
}

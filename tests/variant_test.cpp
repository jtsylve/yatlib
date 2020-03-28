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
#include <yatlib/variant.hpp>

#include "common.hpp"

TEST_CASE("bad_variant_access", "[variant][bad_variant_access]") {
  REQUIRE(yat::bad_variant_access().what() != nullptr);

  constexpr auto do_throw = [] { throw yat::bad_variant_access(); };

  REQUIRE_THROWS_AS(do_throw(), yat::bad_variant_access);

  yat::variant<std::monostate, int> v = 1;

  constexpr auto visitor = [](const auto& val) {
    using type = std::decay_t<decltype(val)>;

    if constexpr (std::is_same_v<type, int>) {
      return true;
    } else {
      return false;
    }
  };

  REQUIRE(yat::visit(visitor, v));
}

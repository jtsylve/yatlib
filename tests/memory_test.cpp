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
#include <yatlib/memory.hpp>

#include "common.hpp"

// Tests adapted from
// https://github.com/llvm/llvm-project/blob/master/libcxx/test/std/utilities/memory/pointer.conversion/to_address.pass.cpp

class P1 {
 public:
  using element_type = int;

  explicit P1(int* p) : p_(p) {}

  int* operator->() const noexcept { return p_; }

 private:
  int* p_;
};

class P2 {
 public:
  using element_type = int;

  explicit P2(int* p) : p_(p) {}

  P1 operator->() const noexcept { return p_; }

 private:
  P1 p_;
};

class P3 {
 public:
  explicit P3(int* p) : p_(p) {}

  int* get() const noexcept { return p_; }

 private:
  int* p_;
};

namespace std {
template <>
struct pointer_traits<::P3> {
  static int* to_address(const ::P3& p) noexcept { return p.get(); }
};
}  // namespace std

class P4 {
 public:
  explicit P4(int* p) : p_(p) {}

  int* operator->() const noexcept { return nullptr; }

  int* get() const noexcept { return p_; }

 private:
  int* p_;
};

namespace std {
template <>
struct pointer_traits<::P4> {
  static int* to_address(const ::P4& p) noexcept { return p.get(); }
};
}  // namespace std

TEST_CASE("to_address", "[memory][to_address]") {
  int i = 0;
  STATIC_REQUIRE(yat::to_address(&i) == &i);

  REQUIRE(yat::to_address(&i) != nullptr);

  P1 p1{&i};
  REQUIRE(yat::to_address(p1) == &i);

  P2 p2{&i};
  REQUIRE(yat::to_address(p2) == &i);

  P3 p3{&i};
  REQUIRE(yat::to_address(p3) == &i);

  P4 p4{&i};
  REQUIRE(yat::to_address(p4) == &i);

  std::array<int, 1> a{};
  REQUIRE(yat::to_address(a.cbegin()) == &a[0]);
}

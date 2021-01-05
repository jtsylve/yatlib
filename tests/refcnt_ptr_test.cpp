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
#include <map>
#include <type_traits>
#include <vector>
#include <yatlib/memory.hpp>

#include "common.hpp"

// Tests adapted from
// https://github.com/boostorg/smart_ptr/blob/develop/test/shared_ptr_test.cpp

TEST_CASE("default constructor", "[memory][refcnt_ptr]") {
  class incomplete;

  yat::refcnt_ptr<int> pi;
  REQUIRE((pi ? false : true));
  REQUIRE(!pi);
  REQUIRE(pi.get() == 0);
  REQUIRE(pi.use_count() == 0);

// refcnt_ptr does not currently support type-erased pointers
#if 0
  yat::refcnt_ptr<void> pv;
  REQUIRE((pv ? false : true));
  REQUIRE(!pv);
  REQUIRE(pv.get() == 0);
  REQUIRE(pv.use_count() == 0);
#endif

  yat::refcnt_ptr<incomplete> px;
  REQUIRE((px ? false : true));
  REQUIRE(!px);
  REQUIRE(px.get() == 0);
  REQUIRE(px.use_count() == 0);
}

namespace constructor {
struct A {
  int dummy;
};

struct X {
  static long instances;

  X() { ++instances; }

  virtual ~X() { --instances; }

 private:
  X(X const &);
  X &operator=(X const &);
};

long X::instances = 0;

// virtual inheritance stresses the implementation

struct Y : public A, public virtual X {
  static long instances;

  Y() { ++instances; }

  ~Y() { --instances; }

 private:
  Y(Y const &);
  Y &operator=(Y const &);
};

long Y::instances = 0;
}  // namespace constructor

template <class T>
void pc0_test(T *p) {
  REQUIRE(p == 0);
  yat::refcnt_ptr<T> pt(p);
  REQUIRE((pt ? false : true));
  REQUIRE(!pt);
  REQUIRE(pt.get() == 0);
  REQUIRE(pt.use_count() == 1);
  REQUIRE(pt.unique());
}

TEST_CASE("pointer constructor", "[memory][refcnt_ptr]") {
  using namespace constructor;

  pc0_test(static_cast<int *>(0));

  pc0_test(static_cast<int const *>(0));
  pc0_test(static_cast<int volatile *>(0));
  pc0_test(static_cast<int const volatile *>(0));

  {
    yat::refcnt_ptr<int const> pi(static_cast<int *>(0));
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());
  }

  {
    yat::refcnt_ptr<int volatile> pi(static_cast<int *>(0));
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> pv(static_cast<int *>(0));
    REQUIRE((pv ? false : true));
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
  }

  {
    yat::refcnt_ptr<void const> pv(static_cast<int *>(0));
    REQUIRE((pv ? false : true));
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
  }
#endif

  pc0_test(static_cast<X *>(0));
  pc0_test(static_cast<X const *>(0));
  pc0_test(static_cast<X volatile *>(0));
  pc0_test(static_cast<X const volatile *>(0));

  {
    yat::refcnt_ptr<X const> px(static_cast<X *>(0));
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
  }

  {
    yat::refcnt_ptr<X> px(static_cast<Y *>(0));
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
  }

  {
    yat::refcnt_ptr<X const> px(static_cast<Y *>(0));
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> pv(static_cast<X *>(0));
    REQUIRE((pv ? false : true));
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
  }

  {
    yat::refcnt_ptr<void const> pv(static_cast<X *>(0));
    REQUIRE((pv ? false : true));
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
  }
#endif

  {
    int *p = new int(7);
    yat::refcnt_ptr<int> pi(p);
    REQUIRE((pi ? true : false));
    REQUIRE(!!pi);
    REQUIRE(pi.get() == p);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());
    REQUIRE(*pi == 7);
  }

  {
    int *p = new int(7);
    yat::refcnt_ptr<int const> pi(p);
    REQUIRE((pi ? true : false));
    REQUIRE(!!pi);
    REQUIRE(pi.get() == p);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());
    REQUIRE(*pi == 7);
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    int *p = new int(7);
    yat::refcnt_ptr<void> pv(p);
    REQUIRE((pv ? true : false));
    REQUIRE(!!pv);
    REQUIRE(pv.get() == p);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
  }

  {
    int *p = new int(7);
    yat::refcnt_ptr<void const> pv(p);
    REQUIRE((pv ? true : false));
    REQUIRE(!!pv);
    REQUIRE(pv.get() == p);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
  }
#endif

  REQUIRE(X::instances == 0);

  {
    X *p = new X;
    yat::refcnt_ptr<X> px(p);
    REQUIRE((px ? true : false));
    REQUIRE(!!px);
    REQUIRE(px.get() == p);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 1);
  }

  REQUIRE(X::instances == 0);

  {
    X *p = new X;
    yat::refcnt_ptr<X const> px(p);
    REQUIRE((px ? true : false));
    REQUIRE(!!px);
    REQUIRE(px.get() == p);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 1);
  }

  REQUIRE(X::instances == 0);

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    X *p = new X;
    yat::refcnt_ptr<void> pv(p);
    REQUIRE((pv ? true : false));
    REQUIRE(!!pv);
    REQUIRE(pv.get() == p);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 1);
  }
#endif

  REQUIRE(X::instances == 0);

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    X *p = new X;
    yat::refcnt_ptr<void const> pv(p);
    REQUIRE((pv ? true : false));
    REQUIRE(!!pv);
    REQUIRE(pv.get() == p);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 1);
  }
#endif

  REQUIRE(X::instances == 0);
  REQUIRE(Y::instances == 0);

  {
    Y *p = new Y;
    yat::refcnt_ptr<X> px(p);
    REQUIRE((px ? true : false));
    REQUIRE(!!px);
    REQUIRE(px.get() == p);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);
  }

  REQUIRE(X::instances == 0);
  REQUIRE(Y::instances == 0);

  {
    Y *p = new Y;
    yat::refcnt_ptr<X const> px(p);
    REQUIRE((px ? true : false));
    REQUIRE(!!px);
    REQUIRE(px.get() == p);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);
  }

  REQUIRE(X::instances == 0);
  REQUIRE(Y::instances == 0);
}

namespace assignment {

struct incomplete;

struct A {
  int dummy;
};

struct X {
  static long instances;

  X() { ++instances; }

  virtual ~X() { --instances; }

 private:
  X(X const &);
  X &operator=(X const &);
};

long X::instances = 0;

struct Y : public A, public virtual X {
  static long instances;

  Y() { ++instances; }

  ~Y() { --instances; }

 private:
  Y(Y const &);
  Y &operator=(Y const &);
};

long Y::instances = 0;
}  // namespace assignment

TEST_CASE("copy assignment", "[memory][refcnt_ptr]") {
  using namespace assignment;

  {
    yat::refcnt_ptr<incomplete> p1;

    p1 = p1;

    REQUIRE(p1 == p1);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<incomplete> p2;

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<incomplete> p3(p1);

    p1 = p3;

    REQUIRE(p1 == p3);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> p1;

    p1 = p1;

    REQUIRE(p1 == p1);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<void> p2;

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<void> p3(p1);

    p1 = p3;

    REQUIRE(p1 == p3);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<void> p4(new int);
    REQUIRE(p4.use_count() == 1);

    p1 = p4;

    REQUIRE(p1 == p4);
    REQUIRE(!(p1 < p4 || p4 < p1));
    REQUIRE(p1.use_count() == 2);
    REQUIRE(p4.use_count() == 2);

    p1 = p3;

    REQUIRE(p1 == p3);
    REQUIRE(p4.use_count() == 1);
  }
#endif

  {
    yat::refcnt_ptr<X> p1;

    p1 = p1;

    REQUIRE(p1 == p1);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<X> p2;

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<X> p3(p1);

    p1 = p3;

    REQUIRE(p1 == p3);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    REQUIRE(X::instances == 0);

    yat::refcnt_ptr<X> p4(new X);

    REQUIRE(X::instances == 1);

    p1 = p4;

    REQUIRE(X::instances == 1);

    REQUIRE(p1 == p4);
    REQUIRE(!(p1 < p4 || p4 < p1));

    REQUIRE(p1.use_count() == 2);

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE(X::instances == 1);

    p4 = p3;

    REQUIRE(p4 == p3);
    REQUIRE(X::instances == 0);
  }
}

TEST_CASE("move assignment", "[memory][refcnt_ptr]") {
  using namespace assignment;

  {
    yat::refcnt_ptr<incomplete> p1;

    p1 = std::move(p1);

    REQUIRE(p1 == p1);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<incomplete> p2;

    p1 = std::move(p2);

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<incomplete> p3(p1);

    p1 = std::move(p3);

    REQUIRE(p1 == p3);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> p1;

    p1 = std::move(p1);

    REQUIRE(p1 == p1);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<void> p2;

    p1 = std::move(p2);

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<void> p3(p1);

    p1 = std::move(p3);

    REQUIRE(p1 == p3);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<void> p4(new int);
    REQUIRE(p4.use_count() == 1);

    p1 = std::move(p4);

    REQUIRE(p1 != p4);
    REQUIRE((p1 < p4 || p4 < p1));
    REQUIRE(p1.use_count() == 1);
    REQUIRE(p4.use_count() == 0);

    p1 = std::move(p3);

    REQUIRE(p1 == p3);
  }
#endif

  {
    yat::refcnt_ptr<X> p1;

    p1 = std::move(p1);

    REQUIRE(p1 == p1);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<X> p2;

    p1 = std::move(p2);

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<X> p3(p1);

    p1 = std::move(p3);

    REQUIRE(p1 == p3);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    REQUIRE(X::instances == 0);

    yat::refcnt_ptr<X> p4(new X);

    REQUIRE(X::instances == 1);

    p1 = std::move(p4);

    REQUIRE(X::instances == 1);

    REQUIRE(p1 != p4);
    REQUIRE((p1 < p4 || p4 < p1));

    REQUIRE(p1.use_count() == 1);
    REQUIRE(p4.use_count() == 0);

    p1 = std::move(p2);

    REQUIRE(p1 == p2);
    REQUIRE(X::instances == 0);

    p4 = std::move(p3);

    REQUIRE(p4 == p3);
    REQUIRE(X::instances == 0);
  }
}

TEST_CASE("conversion assignment", "[memory][refcnt_ptr]") {
  using namespace assignment;

  // refcnt_ptr does not currently support type-erased pointers
#if 0 
  {
    yat::refcnt_ptr<void> p1;

    yat::refcnt_ptr<incomplete> p2;

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    yat::refcnt_ptr<int> p4(new int);
    REQUIRE(p4.use_count() == 1);

    yat::refcnt_ptr<void> p5(p4);
    REQUIRE(p4.use_count() == 2);

    p1 = p4;

    REQUIRE(p1 == p4);
    REQUIRE(!(p1 < p5 || p5 < p1));
    REQUIRE(p1.use_count() == 3);
    REQUIRE(p4.use_count() == 3);

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE(p4.use_count() == 2);
  }
#endif

  {
    yat::refcnt_ptr<X> p1;

    yat::refcnt_ptr<Y> p2;

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE((p1 ? false : true));
    REQUIRE(!p1);
    REQUIRE(p1.get() == 0);

    REQUIRE(X::instances == 0);
    REQUIRE(Y::instances == 0);

    yat::refcnt_ptr<Y> p4(new Y);

    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);
    REQUIRE(p4.use_count() == 1);

    yat::refcnt_ptr<X> p5(p4);
    REQUIRE(p4.use_count() == 2);

    p1 = p4;

    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);

    REQUIRE(p1 == p4);
    REQUIRE(!(p1 < p5 || p5 < p1));

    REQUIRE(p1.use_count() == 3);
    REQUIRE(p4.use_count() == 3);

    p1 = p2;

    REQUIRE(p1 == p2);
    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);
    REQUIRE(p4.use_count() == 2);

    p4 = p2;
    p5 = p2;

    REQUIRE(p4 == p2);
    REQUIRE(X::instances == 0);
    REQUIRE(Y::instances == 0);
  }
}

namespace reset {

class incomplete;

incomplete *p0 = 0;

void deleter(incomplete *) {}

struct X {
  static long instances;

  X() { ++instances; }

  virtual ~X() { --instances; }

 private:
  X(X const &);
  X &operator=(X const &);
};

long X::instances = 0;

}  // namespace reset

TEST_CASE("plain reset", "[memory][refcnt_ptr]") {
  using namespace reset;

  {
    yat::refcnt_ptr<int> pi;
    pi.reset();
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 0);
  }

  {
    yat::refcnt_ptr<int> pi(static_cast<int *>(0));
    pi.reset();
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 0);
  }

  {
    yat::refcnt_ptr<int> pi(new int);
    pi.reset();
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 0);
  }

  {
    yat::refcnt_ptr<incomplete> px;
    px.reset();
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 0);
  }

// refcnt_ptr doesn't support custom deleters
#if 0
  {
    yat::refcnt_ptr<incomplete> px(p0, deleter);
    px.reset();
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 0);
  }
#endif

  {
    yat::refcnt_ptr<X> px;
    px.reset();
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 0);
  }

  {
    REQUIRE(X::instances == 0);
    yat::refcnt_ptr<X> px(new X);
    REQUIRE(X::instances == 1);
    px.reset();
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 0);
    REQUIRE(X::instances == 0);
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> pv;
    pv.reset();
    REQUIRE((pv ? false : true));
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 0);
  }

  {
    REQUIRE(X::instances == 0);
    yat::refcnt_ptr<void> pv(new X);
    REQUIRE(X::instances == 1);
    pv.reset();
    REQUIRE((pv ? false : true));
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 0);
    REQUIRE(X::instances == 0);
  }
#endif
}

namespace reset {
struct A {
  int dummy;
};

struct Y : public A, public virtual X {
  static long instances;

  Y() { ++instances; }

  ~Y() { --instances; }

 private:
  Y(Y const &);
  Y &operator=(Y const &);
};

long Y::instances = 0;
}  // namespace reset

TEST_CASE("pointer reset", "[memory][refcnt_ptr]") {
  using namespace reset;

  {
    yat::refcnt_ptr<int> pi;

    pi.reset(static_cast<int *>(0));
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());

    int *p = new int;
    pi.reset(p);
    REQUIRE((pi ? true : false));
    REQUIRE(!!pi);
    REQUIRE(pi.get() == p);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());

    pi.reset(static_cast<int *>(0));
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());
  }

  {
    yat::refcnt_ptr<X> px;

    px.reset(static_cast<X *>(0));
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 0);

    X *p = new X;
    px.reset(p);
    REQUIRE((px ? true : false));
    REQUIRE(!!px);
    REQUIRE(px.get() == p);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 1);

    px.reset(static_cast<X *>(0));
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 0);
    REQUIRE(Y::instances == 0);

    Y *q = new Y;
    px.reset(q);
    REQUIRE((px ? true : false));
    REQUIRE(!!px);
    REQUIRE(px.get() == q);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);

    px.reset(static_cast<Y *>(0));
    REQUIRE((px ? false : true));
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());
    REQUIRE(X::instances == 0);
    REQUIRE(Y::instances == 0);
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> pv;

    pv.reset(static_cast<X *>(0));
    REQUIRE(pv ? false : true);
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 0);

    X *p = new X;
    pv.reset(p);
    REQUIRE(pv ? true : false);
    REQUIRE(!!pv);
    REQUIRE(pv.get() == p);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 1);

    pv.reset(static_cast<X *>(0));
    REQUIRE(pv ? false : true);
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 0);
    REQUIRE(Y::instances == 0);

    Y *q = new Y;
    pv.reset(q);
    REQUIRE(pv ? true : false);
    REQUIRE(!!pv);
    REQUIRE(pv.get() == q);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 1);
    REQUIRE(Y::instances == 1);

    pv.reset(static_cast<Y *>(0));
    REQUIRE(pv ? false : true);
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());
    REQUIRE(X::instances == 0);
    REQUIRE(Y::instances == 0);
  }
#endif
}

// refcnt_ptr doesn't currently support custom deleters
#if 0
namespace reset {
void *deleted = 0;

void deleter2(void *p) { deleted = p; }
}  // namespace reset

void TEST_CASE("deleted reset", "[memory][refcnt_ptr]") {
  using namespace reset;
  {
    yat::refcnt_ptr<int> pi;

    pi.reset(static_cast<int *>(0), deleter2);
    REQUIRE((pi ? false : true));
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());

    deleted = &pi;

    int m = 0;
    pi.reset(&m, deleter2);
    REQUIRE(deleted == 0);
    REQUIRE((pi ? true : false));
    REQUIRE(!!pi);
    REQUIRE(pi.get() == &m);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());

    pi.reset(static_cast<int *>(0), deleter2);
    REQUIRE(deleted == &m);
    REQUIRE(pi ? false : true);
    REQUIRE(!pi);
    REQUIRE(pi.get() == 0);
    REQUIRE(pi.use_count() == 1);
    REQUIRE(pi.unique());

    pi.reset();
    REQUIRE(deleted == 0);
  }

  {
    yat::refcnt_ptr<X> px;

    px.reset(static_cast<X *>(0), deleter2);
    REQUIRE(px ? false : true);
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    deleted = &px;

    X x;
    px.reset(&x, deleter2);
    REQUIRE(deleted == 0);
    REQUIRE(px ? true : false);
    REQUIRE(!!px);
    REQUIRE(px.get() == &x);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    px.reset(static_cast<X *>(0), deleter2);
    REQUIRE(deleted == &x);
    REQUIRE(px ? false : true);
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    Y y;
    px.reset(&y, deleter2);
    REQUIRE(deleted == 0);
    REQUIRE(px ? true : false);
    REQUIRE(!!px);
    REQUIRE(px.get() == &y);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    px.reset(static_cast<Y *>(0), deleter2);
    REQUIRE(deleted == &y);
    REQUIRE(px ? false : true);
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    px.reset();
    REQUIRE(deleted == 0);
  }

  {
    yat::refcnt_ptr<void> pv;

    pv.reset(static_cast<X *>(0), deleter2);
    REQUIRE(pv ? false : true);
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());

    deleted = &pv;

    X x;
    pv.reset(&x, deleter2);
    REQUIRE(deleted == 0);
    REQUIRE(pv ? true : false);
    REQUIRE(!!pv);
    REQUIRE(pv.get() == &x);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());

    pv.reset(static_cast<X *>(0), deleter2);
    REQUIRE(deleted == &x);
    REQUIRE(pv ? false : true);
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());

    Y y;
    pv.reset(&y, deleter2);
    REQUIRE(deleted == 0);
    REQUIRE(pv ? true : false);
    REQUIRE(!!pv);
    REQUIRE(pv.get() == &y);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());

    pv.reset(static_cast<Y *>(0), deleter2);
    REQUIRE(deleted == &y);
    REQUIRE(pv ? false : true);
    REQUIRE(!pv);
    REQUIRE(pv.get() == 0);
    REQUIRE(pv.use_count() == 1);
    REQUIRE(pv.unique());

    pv.reset();
    REQUIRE(deleted == 0);
  }

  {
    yat::refcnt_ptr<incomplete> px;

    px.reset(p0, deleter2);
    REQUIRE(px ? false : true);
    REQUIRE(!px);
    REQUIRE(px.get() == 0);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    deleted = &px;
    px.reset(p0, deleter2);
    REQUIRE(deleted == 0);
  }
}
#endif

TEST_CASE("use count", "[memory][refcnt_ptr]") {
  struct X {};

  {
    yat::refcnt_ptr<X> px(static_cast<X *>(0));
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    yat::refcnt_ptr<X> px2(px);
    REQUIRE(px2.use_count() == 2);
    REQUIRE(!px2.unique());
    REQUIRE(px.use_count() == 2);
    REQUIRE(!px.unique());
  }

  {
    yat::refcnt_ptr<X> px(new X);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    yat::refcnt_ptr<X> px2(px);
    REQUIRE(px2.use_count() == 2);
    REQUIRE(!px2.unique());
    REQUIRE(px.use_count() == 2);
    REQUIRE(!px.unique());
  }

// refcnt_ptr doesn't support custom deleters
#if 0
  {
    yat::refcnt_ptr<X> px(new X, boost::checked_deleter<X>());
    REQUIRE(px.use_count() == 1);
    REQUIRE(px.unique());

    yat::refcnt_ptr<X> px2(px);
    REQUIRE(px2.use_count() == 2);
    REQUIRE(!px2.unique());
    REQUIRE(px.use_count() == 2);
    REQUIRE(!px.unique());
  }
#endif
}

TEST_CASE("swap", "[memory][refcnt_ptr]") {
  struct X {};

  {
    yat::refcnt_ptr<X> px;
    yat::refcnt_ptr<X> px2;

    px.swap(px2);

    REQUIRE(px.get() == 0);
    REQUIRE(px2.get() == 0);

    using std::swap;
    swap(px, px2);

    REQUIRE(px.get() == 0);
    REQUIRE(px2.get() == 0);
  }

  {
    X *p = new X;
    yat::refcnt_ptr<X> px;
    yat::refcnt_ptr<X> px2(p);
    yat::refcnt_ptr<X> px3(px2);

    px.swap(px2);

    REQUIRE(px.get() == p);
    REQUIRE(px.use_count() == 2);
    REQUIRE(px2.get() == 0);
    REQUIRE(px3.get() == p);
    REQUIRE(px3.use_count() == 2);

    using std::swap;
    swap(px, px2);

    REQUIRE(px.get() == 0);
    REQUIRE(px2.get() == p);
    REQUIRE(px2.use_count() == 2);
    REQUIRE(px3.get() == p);
    REQUIRE(px3.use_count() == 2);
  }

  {
    X *p1 = new X;
    X *p2 = new X;
    yat::refcnt_ptr<X> px(p1);
    yat::refcnt_ptr<X> px2(p2);
    yat::refcnt_ptr<X> px3(px2);

    px.swap(px2);

    REQUIRE(px.get() == p2);
    REQUIRE(px.use_count() == 2);
    REQUIRE(px2.get() == p1);
    REQUIRE(px2.use_count() == 1);
    REQUIRE(px3.get() == p2);
    REQUIRE(px3.use_count() == 2);

    using std::swap;
    swap(px, px2);

    REQUIRE(px.get() == p1);
    REQUIRE(px.use_count() == 1);
    REQUIRE(px2.get() == p2);
    REQUIRE(px2.use_count() == 2);
    REQUIRE(px3.get() == p2);
    REQUIRE(px3.use_count() == 2);
  }
}

TEST_CASE("null comparison", "[memory][refcnt_ptr]") {
  {
    yat::refcnt_ptr<int> x;

    REQUIRE(x == nullptr);
    REQUIRE(nullptr == x);
    REQUIRE(!(x != nullptr));
    REQUIRE(!(nullptr != x));

    REQUIRE(!(x < nullptr || x > nullptr));
    REQUIRE(!(nullptr < x || nullptr > x));
    REQUIRE((x <= nullptr && x >= nullptr));
    REQUIRE((nullptr <= x && nullptr >= x));
  }

  {
    yat::refcnt_ptr<int> x(new int);

    REQUIRE(x != nullptr);
    REQUIRE(nullptr != x);
    REQUIRE(!(x == nullptr));
    REQUIRE(!(nullptr == x));

    REQUIRE((nullptr < x && x > nullptr));
    REQUIRE(!(nullptr > x || x < nullptr));
    REQUIRE((nullptr <= x && x >= nullptr));
    REQUIRE(!(nullptr >= x || x <= nullptr));
  }
}

TEST_CASE("comparison", "[memory][refcnt_ptr]") {
  struct X {
    int dummy;
  };

  struct Y {
    int dummy2;
  };

  struct Z : public X, public virtual Y {};

  {
    yat::refcnt_ptr<X> px;
    REQUIRE(px == px);
    REQUIRE(!(px != px));
    REQUIRE(!(px < px));

    yat::refcnt_ptr<X> px2;

    REQUIRE(px.get() == px2.get());
    REQUIRE(px == px2);
    REQUIRE(!(px != px2));
    REQUIRE(!(px < px2 && px2 < px));
  }

  {
    yat::refcnt_ptr<X> px;
    yat::refcnt_ptr<X> px2(px);

    REQUIRE(px2 == px2);
    REQUIRE(!(px2 != px2));
    REQUIRE(!(px2 < px2));

    REQUIRE(px.get() == px2.get());
    REQUIRE(px == px2);
    REQUIRE(!(px != px2));
    REQUIRE(!(px < px2 && px2 < px));
  }

  {
    yat::refcnt_ptr<X> px;
    yat::refcnt_ptr<X> px2(new X);

    REQUIRE(px2 == px2);
    REQUIRE(!(px2 != px2));
    REQUIRE(!(px2 < px2));

    REQUIRE(px.get() != px2.get());
    REQUIRE(px != px2);
    REQUIRE(!(px == px2));
    REQUIRE((px < px2 || px2 < px));
    REQUIRE(!(px < px2 && px2 < px));
  }

  {
    yat::refcnt_ptr<X> px(new X);
    yat::refcnt_ptr<X> px2(new X);

    REQUIRE(px.get() != px2.get());
    REQUIRE(px != px2);
    REQUIRE(!(px == px2));
    REQUIRE((px < px2 || px2 < px));
    REQUIRE(!(px < px2 && px2 < px));
  }

  {
    yat::refcnt_ptr<X> px(new X);
    yat::refcnt_ptr<X> px2(px);

    REQUIRE(px2 == px2);
    REQUIRE(!(px2 != px2));
    REQUIRE(!(px2 < px2));

    REQUIRE(px.get() == px2.get());
    REQUIRE(px == px2);
    REQUIRE(!(px != px2));
    REQUIRE(!(px < px2 || px2 < px));
  }

  {
    yat::refcnt_ptr<X> px(new X);
    yat::refcnt_ptr<Y> py(new Y);
    yat::refcnt_ptr<Z> pz(new Z);

    REQUIRE(px.get() != pz.get());
    REQUIRE(px != pz);
    REQUIRE(!(px == pz));

    REQUIRE(py.get() != pz.get());
    REQUIRE(py != pz);
    REQUIRE(!(py == pz));

    REQUIRE((px < py || py < px));
    REQUIRE((px < pz || pz < px));
    REQUIRE((py < pz || pz < py));

    REQUIRE(!(px < py && py < px));
    REQUIRE(!(px < pz && pz < px));
    REQUIRE(!(py < pz && pz < py));

// refcnt_ptr does not currently support type-erased pointers
#if 0
    yat::refcnt_ptr<void> pvx(px);

    REQUIRE(pvx == pvx);
    REQUIRE(!(pvx != pvx));
    REQUIRE(!(pvx < pvx));

    yat::refcnt_ptr<void> pvy(py);
    yat::refcnt_ptr<void> pvz(pz);

    REQUIRE((pvx < pvy || pvy < pvx));
    REQUIRE((pvx < pvz || pvz < pvx));
    REQUIRE((pvy < pvz || pvz < pvy));

    REQUIRE(!(pvx < pvy && pvy < pvx));
    REQUIRE(!(pvx < pvz && pvz < pvx));
    REQUIRE(!(pvy < pvz && pvz < pvy));
#endif
  }

  {
    yat::refcnt_ptr<Z> pz(new Z);
    yat::refcnt_ptr<X> px(pz);

    REQUIRE(px == px);
    REQUIRE(!(px != px));
    REQUIRE(!(px < px));

    yat::refcnt_ptr<Y> py(pz);

    REQUIRE(px.get() == pz.get());
    REQUIRE(px == pz);
    REQUIRE(!(px != pz));

    REQUIRE(py.get() == pz.get());
    REQUIRE(py == pz);
    REQUIRE(!(py != pz));

    REQUIRE(!(px < py || py < px));
    REQUIRE(!(px < pz || pz < px));
    REQUIRE(!(py < pz || pz < py));

// refcnt_ptr does not currently support type-erased pointers
#if 0
    yat::refcnt_ptr<void> pvx(px);
    yat::refcnt_ptr<void> pvy(py);
    yat::refcnt_ptr<void> pvz(pz);

    // pvx and pvy aren't equal...
    REQUIRE(pvx.get() != pvy.get());
    REQUIRE(pvx != pvy);
    REQUIRE(!(pvx == pvy));

    // ... but they share ownership ...
    REQUIRE(!(pvx < pvy || pvy < pvx));

    // ... with pvz
    REQUIRE(!(pvx < pvz || pvz < pvx));
    REQUIRE(!(pvy < pvz || pvz < pvy));
#endif
  }
}

TEST_CASE("static cast", "[memory][refcnt_ptr]") {
  struct X {};

  struct Y : public X {};

  // refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void> pv;

    yat::refcnt_ptr<int> pi = yat::static_pointer_cast<int>(pv);
    REQUIRE(pi.get() == 0);

    yat::refcnt_ptr<X> px = yat::static_pointer_cast<X>(pv);
    REQUIRE(px.get() == 0);
  }

  {
    yat::refcnt_ptr<int> pi(new int);
    yat::refcnt_ptr<void> pv(pi);

    yat::refcnt_ptr<int> pi2 = yat::static_pointer_cast<int>(pv);
    REQUIRE(pi.get() == pi2.get());
    REQUIRE(!(pi < pi2 || pi2 < pi));
    REQUIRE(pi.use_count() == 3);
    REQUIRE(pv.use_count() == 3);
    REQUIRE(pi2.use_count() == 3);
  }

  {
    yat::refcnt_ptr<X> px(new X);
    yat::refcnt_ptr<void> pv(px);

    yat::refcnt_ptr<X> px2 = yat::static_pointer_cast<X>(pv);
    REQUIRE(px.get() == px2.get());
    REQUIRE(!(px < px2 || px2 < px));
    REQUIRE(px.use_count() == 3);
    REQUIRE(pv.use_count() == 3);
    REQUIRE(px2.use_count() == 3);
  }
#endif

  {
    yat::refcnt_ptr<X> px(new Y);

    yat::refcnt_ptr<Y> py = yat::static_pointer_cast<Y>(px);
    REQUIRE(px.get() == py.get());
    REQUIRE(px.use_count() == 2);
    REQUIRE(py.use_count() == 2);

    yat::refcnt_ptr<X> px2(py);
    REQUIRE(!(px < px2 || px2 < px));
  }
}

TEST_CASE("const cast", "[memory][refcnt_ptr]") {
  struct X;

  // refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void const volatile> px;

    yat::refcnt_ptr<void> px2 = yat::const_pointer_cast<void>(px);
    REQUIRE(px2.get() == 0);
  }
#endif

  {
    yat::refcnt_ptr<int const volatile> px;

    yat::refcnt_ptr<int> px2 = yat::const_pointer_cast<int>(px);
    REQUIRE(px2.get() == 0);
  }

  {
    yat::refcnt_ptr<X const volatile> px;

    yat::refcnt_ptr<X> px2 = yat::const_pointer_cast<X>(px);
    REQUIRE(px2.get() == 0);
  }

  // refcnt_ptr does not currently support type-erased pointers
#if 0
  {
    yat::refcnt_ptr<void const volatile> px(new int);

    yat::refcnt_ptr<void> px2 = yat::const_pointer_cast<void>(px);
    REQUIRE(px.get() == px2.get());
    REQUIRE(!(px < px2 || px2 < px));
    REQUIRE(px.use_count() == 2);
    REQUIRE(px2.use_count() == 2);
  }
#endif

  {
    yat::refcnt_ptr<int const volatile> px(new int);

    yat::refcnt_ptr<int> px2 = yat::const_pointer_cast<int>(px);
    REQUIRE(px.get() == px2.get());
    REQUIRE(!(px < px2 || px2 < px));
    REQUIRE(px.use_count() == 2);
    REQUIRE(px2.use_count() == 2);
  }
}

TEST_CASE("dynamic cast", "[memory][refcnt_ptr]") {
  struct V {
    virtual ~V() {}
  };

  struct W : public V {};

  {
    yat::refcnt_ptr<V> pv;
    yat::refcnt_ptr<W> pw = yat::dynamic_pointer_cast<W>(pv);
    REQUIRE(pw.get() == 0);
  }

  {
    yat::refcnt_ptr<V> pv(static_cast<V *>(0));

    yat::refcnt_ptr<W> pw = yat::dynamic_pointer_cast<W>(pv);
    REQUIRE(pw.get() == 0);

    yat::refcnt_ptr<V> pv2(pw);
    REQUIRE((pv < pv2 || pv2 < pv));
  }

  {
    yat::refcnt_ptr<V> pv(static_cast<W *>(0));

    yat::refcnt_ptr<W> pw = yat::dynamic_pointer_cast<W>(pv);
    REQUIRE(pw.get() == 0);

    yat::refcnt_ptr<V> pv2(pw);
    REQUIRE((pv < pv2 || pv2 < pv));
  }

  {
    yat::refcnt_ptr<V> pv(new V);

    yat::refcnt_ptr<W> pw = yat::dynamic_pointer_cast<W>(pv);
    REQUIRE(pw.get() == 0);

    yat::refcnt_ptr<V> pv2(pw);
    REQUIRE((pv < pv2 || pv2 < pv));
  }

  {
    yat::refcnt_ptr<V> pv(new W);

    yat::refcnt_ptr<W> pw = yat::dynamic_pointer_cast<W>(pv);
    REQUIRE(pw.get() == pv.get());
    REQUIRE(pv.use_count() == 2);
    REQUIRE(pw.use_count() == 2);

    yat::refcnt_ptr<V> pv2(pw);
    REQUIRE(!(pv < pv2 || pv2 < pv));
  }
}

TEST_CASE("containers", "[memory][refcnt_ptr]") {
  struct X {};

  std::vector<yat::refcnt_ptr<int> > vi;

  {
    yat::refcnt_ptr<int> pi1(new int);
    yat::refcnt_ptr<int> pi2(new int);
    yat::refcnt_ptr<int> pi3(new int);

    vi.push_back(pi1);
    vi.push_back(pi1);
    vi.push_back(pi1);
    vi.push_back(pi2);
    vi.push_back(pi1);
    vi.push_back(pi2);
    vi.push_back(pi1);
    vi.push_back(pi3);
    vi.push_back(pi3);
    vi.push_back(pi2);
    vi.push_back(pi1);
  }

  std::vector<yat::refcnt_ptr<X> > vx;

  {
    yat::refcnt_ptr<X> px1(new X);
    yat::refcnt_ptr<X> px2(new X);
    yat::refcnt_ptr<X> px3(new X);

    vx.push_back(px2);
    vx.push_back(px2);
    vx.push_back(px1);
    vx.push_back(px2);
    vx.push_back(px1);
    vx.push_back(px1);
    vx.push_back(px1);
    vx.push_back(px2);
    vx.push_back(px1);
    vx.push_back(px3);
    vx.push_back(px2);
  }

// refcnt_ptr does not currently support type-erased pointers
#if 0
  std::map<yat::refcnt_ptr<void>, long> m;

  {
    for (std::vector<yat::refcnt_ptr<int> >::iterator i = vi.begin();
         i != vi.end(); ++i) {
      ++m[*i];
    }
  }

  {
    for (std::vector<yat::refcnt_ptr<X> >::iterator i = vx.begin();
         i != vx.end(); ++i) {
      ++m[*i];
    }
  }

  {
    for (std::map<yat::refcnt_ptr<void>, long>::iterator i = m.begin();
         i != m.end(); ++i) {
      REQUIRE(i->first.use_count() == i->second + 1);
    }
  }
#endif
}

TEST_CASE("transitive", "[memory][refcnt_ptr]") {
  struct X {
    X() : next() {}
    yat::refcnt_ptr<X> next;
  };

  yat::refcnt_ptr<X> p(new X);
  REQUIRE(!p->next);

  p->next = yat::refcnt_ptr<X>(new X);
  REQUIRE(p->next);
  REQUIRE(!p->next->next);

  p = p->next;
  REQUIRE(!p->next);
}

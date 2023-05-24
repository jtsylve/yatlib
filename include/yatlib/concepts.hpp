// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#if __has_include("concepts")
#include <concepts>
#endif

#include <utility>

#include "__compat.hpp"
#include "__config.hpp"
#include "type_traits.hpp"

namespace yat {
inline namespace v2 {

/// The concept same_as_any<T, Ts...> is satisfied if and only if T is the same
/// type as at least one of the types in Ts.
template <typename T, typename... Ts>
concept same_as_any = (same_as<T, Ts> || ...);

/// The concept same_as_none<T, Ts...> is satisfied if and only if T is not the
/// same type as any of the types in Ts.
template <typename T, typename... Ts>
concept same_as_none = not same_as_any<T, Ts...>;

/// The concept same_as_all<T, Ts...> is satisfied if and only if T is the same
/// type as all of the types in Ts.
template <typename T, typename... Ts>
concept same_as_all = (same_as<T, Ts> && ...);

/// The concept char_type<T> is satisfied if and only if T is one of the
/// following types: char, wchar_t, char8_t, char16_t, char32_t.
template <typename T>
concept char_type = same_as_any<T, char, wchar_t, char8_t, char16_t, char32_t>;

/// The concept specialization_of is satisfied if and only if T is a
/// specialization of a templated type, TT.
template <typename T, template <typename...> typename TT>
concept specialization_of = is_specialization_of_v<T, TT>;

/// The concept boolean_testable<T> is satisfied if and only if T is a type that
/// can be converted to bool and the expression !t is well-formed and returns a
/// value that is convertible to bool.
template <typename T>
concept boolean_testable = convertible_to<T, bool> && requires(T&& t) {
  { not std::forward<T>(t) } -> convertible_to<bool>;
};

#pragma region C++20 Standard Concepts
inline namespace YAT_STDCPP20 cpp20 {

#if YAT_HAS_STDLIB_FEATURE(concepts, 202002L)

using std::assignable_from;
using std::common_reference_with;
using std::common_with;
using std::constructible_from;
using std::convertible_to;
using std::copy_constructible;
using std::copyable;
using std::default_initializable;
using std::derived_from;
using std::destructible;
using std::equality_comparable;
using std::equality_comparable_with;
using std::equivalence_relation;
using std::floating_point;
using std::integral;
using std::invocable;
using std::movable;
using std::move_constructible;
using std::predicate;
using std::regular;
using std::regular_invocable;
using std::relation;
using std::same_as;
using std::semiregular;
using std::signed_integral;
using std::strict_weak_order;
using std::swappable;
using std::swappable_with;
using std::totally_ordered;
using std::totally_ordered_with;
using std::unsigned_integral;

namespace ranges {
using std::ranges::swap;
}  // namespace ranges

#else

/// The concept convertible_to<From, To> is satisfied if and only if an
/// expression of type From can be implicitly converted to type To.
///
/// See: https://en.cppreference.com/w/cpp/concepts/convertible_to
template <class From, class To>
concept convertible_to = std::is_convertible_v<From, To> &&
                         requires { static_cast<To>(std::declval<From>()); };

/// The concept derived_from<Derived, Base> is satisfied if and only if Base is
/// a class type that is either Derived or a public and unambiguous base of
/// Derived, ignoring cv-qualifiers.
///
/// Note that this behavior is different to std::is_base_of when Base is a
/// private or protected base of Derived.
///
/// See: https://en.cppreference.com/w/cpp/concepts/derived_from
template <class Derived, class Base>
concept derived_from =
    std::is_base_of_v<Base, Derived> &&
    std::is_convertible_v<const volatile Derived*, const volatile Base*>;

/// The concept same_as<T, U> is satisfied if and only if T and U denote the
/// same type.
///
/// See: https://en.cppreference.com/w/cpp/concepts/same_as
template <class T, class U>
concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;

/// The concept common_reference_with<T, U> specifies that two types T and U
/// share a common reference type (as computed by std::common_reference_t) to
/// which both can be converted.
///
/// See: https://en.cppreference.com/w/cpp/concepts/common_reference_with
template <class T, class U>
concept common_reference_with =
    same_as<common_reference_t<T, U>, common_reference_t<U, T> > &&
    convertible_to<T, common_reference_t<T, U> > &&
    convertible_to<U, common_reference_t<T, U> >;

/// The concept common_with<T, U> specifies that two types T and U share a
/// common type (as computed by std::common_type_t) to which both can be
/// converted.
///
/// See: https://en.cppreference.com/w/cpp/concepts/common_with
template <class T, class U>
concept common_with =
    same_as<std::common_type_t<T, U>, std::common_type_t<U, T> > &&
    requires {
      static_cast<std::common_type_t<T, U> >(std::declval<T>());
      static_cast<std::common_type_t<T, U> >(std::declval<U>());
    } &&
    common_reference_with<std::add_lvalue_reference_t<const T>,
                          std::add_lvalue_reference_t<const U> > &&
    common_reference_with<
        std::add_lvalue_reference_t<std::common_type_t<T, U> >,
        common_reference_t<std::add_lvalue_reference_t<const T>,
                           std::add_lvalue_reference_t<const U> > >;

/// The concept integral<T> is satisfied if and only if T is an integral type.
///
/// See: https://en.cppreference.com/w/cpp/concepts/integral
template <class T>
concept integral = std::is_integral_v<T>;

/// The concept signed_integral<T> is satisfied if and only if T is an integral
/// type and std::is_signed_v<T> is true.
///
/// See: https://en.cppreference.com/w/cpp/concepts/signed_integral
template <class T>
concept signed_integral = integral<T> && std::is_signed_v<T>;

/// The concept unsigned_integral<T> is satisfied if and only if T is an
/// integral type and std::is_signed_v<T> is false.
///
/// See: https://en.cppreference.com/w/cpp/concepts/unsigned_integral
template <class T>
concept unsigned_integral = std::integral<T> && !std::signed_integral<T>;

/// The concept floating_point<T> is satisfied if and only if T is a floating
/// point type.
///
/// See: https://en.cppreference.com/w/cpp/concepts/floating_point
template <class T>
concept floating_point = std::is_floating_point_v<T>;

/// The concept assignable_from<LHS, RHS> specifies that an expression of the
/// type and value category specified by RHS can be assigned to an lvalue
/// expression whose type is specified by LHS.
///
/// See: https://en.cppreference.com/w/cpp/concepts/assignable_from
template <class LHS, class RHS>
concept assignable_from =
    std::is_lvalue_reference_v<LHS> &&
    common_reference_with<const std::remove_reference_t<LHS>&,
                          const std::remove_reference_t<RHS>&> &&
    requires(LHS lhs, RHS&& rhs) {
      { lhs = std::forward<RHS>(rhs) } -> same_as<LHS>;
    };

#pragma region std::ranges::swap
namespace ranges {
namespace __swap {

// needed for overload resolution in __cpo
template <class T>
void swap(T&, T&) = delete;

// Concept __class_or_enum<T> is satisfied if and only if T is a "class type or
// enumeration type" according to the Core wording of the standard.
template <class T>
concept __class_or_enum =
    std::is_class_v<T> || std::is_union_v<T> || std::is_enum_v<T>;

// clang-format off
template <class T1, class T2>
concept __unqualified_swappable_with =
    (__class_or_enum<std::remove_cvref_t<T1>> ||
     __class_or_enum<std::remove_cvref_t<T2>>) &&
    requires(T1&& t1, T2&& t2) {
      swap(std::forward<T1>(t1), std::forward<T2>(t2));
    };
// clang-format on

//
// The name ranges​::​swap denotes a customization point object. The
// expression ranges​::​swap(E1, E2) for subexpressions E1 and E2 is
// expression-equivalent to an expression S determined as follows:
//
struct __cpo {
  //
  // S is `(void)swap(E1, E2)` if E1 or E2 has class or enumeration type and
  // that expression is valid, with overload resolution performed in a context
  // that includes the declaration
  //
  // ```
  // template<class T>
  // void swap(T&, T&) = delete;
  // ```
  //
  // and does not include a declaration of `ranges​::​swap`. If the function
  // selected by overload resolution does not exchange the values denoted by E1
  // and E2, the program is ill-formed, no diagnostic required.
  //
  template <class E1, class E2>
    requires __unqualified_swappable_with<E1, E2>
  constexpr void operator()(E1&& e1, E2&& e2) const
      noexcept(noexcept(swap(std::forward<E1>(e1), std::forward<E2>(e2)))) {
    swap(std::forward<E1>(e1), std::forward<E2>(e2));
  }

  //
  // Otherwise, if E1 and E2 are lvalues of array types  with equal extent and
  // ranges​::​swap(*E1, *E2) is a valid expression, S is
  // (void)ranges​::​swap_ranges(E1, E2), except that noexcept(S) is equal
  // to noexcept(​ranges​::​swap(*E1, *E2)).
  //
  template <class E1, class E2, std::size_t Extent>
    requires requires(E1 (&e1)[Extent], E2 (&e2)[Extent],
                      const __cpo& swapper) { swapper(e1[0], e2[0]); }
  constexpr void operator()(E1 (&e1)[Extent], E2 (&e2)[Extent]) const
      noexcept(noexcept(ranges::swap(*e1, *e2))) {
    // TODO: replace with ranges::swap_ranges
    for (std::size_t i = 0; i < Extent; ++i) {
      (*this)(e1[i], e2[i]);
    }
  }

  //
  // Otherwise, if E1 and E2 are lvalues of the same type T that models
  // move_constructible<T> and assignable_from<T&, T>, S is an expression that
  // exchanges the denoted values.
  //
  template <class T>
    requires move_constructible<T> && assignable_from<T&, T>
  constexpr void operator()(T& e1, T& e2) const
      noexcept(std::is_nothrow_move_constructible_v<T>&&
                   std::is_nothrow_move_assignable_v<T>) {
    e2 = std::exchange(e1, std::move(e2));
  }
};

}  // namespace __swap

inline namespace __cpo {

/// The name ranges​::​swap denotes a customization point object that is
/// used to find the implementation of ​swap.
///
/// See: https://en.cppreference.com/w/cpp/ranges/swap
inline constexpr auto swap = __swap::__cpo{};

}  // namespace __cpo
}  // namespace ranges
#pragma endregion  // std::ranges::swap

/// The concept swappable<T> specifies that lvalues of type T are swappable.
///
/// See: https://en.cppreference.com/w/cpp/concepts/swappable
template <class T>
concept swappable = requires(T& a, T& b) { ranges::swap(a, b); };

/// The concept swappable_with<T, U> specifies that expressions of the type and
/// value category encoded by T and U are swappable with each other.
/// swappable_with<T, U> is satisfied only if a call to ranges::swap(t, u)
/// exchanges the value of t and u, that is, given distinct objects t2 equal to
/// t and u2 equal to u, after evaluating either ranges::swap(t, u) or
/// ranges::swap(u, t), t2 is equal to u and u2 is equal to t.
///
/// See: https://en.cppreference.com/w/cpp/concepts/swappable
template <class T, class U>
concept swappable_with =
    common_reference_with<const std::remove_reference_t<T>&,
                          const std::remove_reference_t<U>&> &&
    requires(T&& t, U&& u) {
      ranges::swap(std::forward<T>(t), std::forward<T>(t));
      ranges::swap(std::forward<U>(u), std::forward<U>(u));
      ranges::swap(std::forward<T>(t), std::forward<U>(u));
      ranges::swap(std::forward<U>(u), std::forward<T>(t));
    };

/// The concept destructible specifies the concept of all types whose instances
/// can safely be destroyed at the end of their lifetime (including reference
/// types).
///
/// See: https://en.cppreference.com/w/cpp/concepts/destructible
template <class T>
concept destructible = std::is_nothrow_destructible_v<T>;

///  The constructible_from concept specifies that a variable of type T can be
///  initialized with the given set of argument types Args....
///
/// See: https://en.cppreference.com/w/cpp/concepts/constructible_from
template <class T, class... Args>
concept constructible_from =
    destructible<T> && std::is_constructible_v<T, Args...>;

/// The default_initializable concept checks whether variables of type T can be
/// default-initialized.
///
/// See: https://en.cppreference.com/w/cpp/concepts/default_initializable
template <class T>
concept default_initializable =
    constructible_from<T> && requires { T{}; } && requires { ::new T; };

/// The concept move_constructible is satisfied if T is a reference type, or if
/// it is an object type where an object of that type can be constructed from an
/// rvalue of that type in both direct- and copy-initialization contexts, with
/// the usual semantics.
///
/// See: https://en.cppreference.com/w/cpp/concepts/move_constructible
template <class T>
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

/// The concept copy_constructible is satisfied if T is an lvalue reference
/// type, or if it is a move_constructible object type where an object of that
/// type can constructed from a (possibly const) lvalue or const rvalue of that
/// type in both direct- and copy-initialization contexts with the usual
/// semantics (a copy is constructed with the source unchanged).
///
/// See: https://en.cppreference.com/w/cpp/concepts/copy_constructible
template <class T>
concept copy_constructible =
    move_constructible<T> && constructible_from<T, T&> &&
    convertible_to<T&, T> && constructible_from<T, const T&> &&
    convertible_to<const T&, T> && constructible_from<T, const T> &&
    convertible_to<const T, T>;

/// The concept __weakly_equality_comparable_with specifies that an object of
/// type T and an object of type U can be compared for equality with each other
/// (in either order) using both == and !=, and the results of the comparisons
/// are consistent.
///
/// See: https://en.cppreference.com/w/cpp/concepts/equality_comparable
template <class T, class U>
concept __weakly_equality_comparable_with = requires(
    const std::remove_reference_t<T>& t, const std::remove_reference_t<U>& u) {
  { t == u } -> boolean_testable;
  { t != u } -> boolean_testable;
  { u == t } -> boolean_testable;
  { u != t } -> boolean_testable;
};

/// The concept std::equality_comparable specifies that the comparison operators
/// == and != on T reflects equality: == yields true if and only if the operands
/// are equal.
///
/// See: https://en.cppreference.com/w/cpp/concepts/equality_comparable
template <class T>
concept equality_comparable = __weakly_equality_comparable_with<T, T>;

/// The concept std::equality_comparable_with specifies that the comparison
/// operators == and != on (possibly mixed) T and U operands yield results
/// consistent with equality. Comparing mixed operands yields results equivalent
/// to comparing the operands converted to their common type.
///
/// See: https://en.cppreference.com/w/cpp/concepts/equality_comparable
template <class T, class U>
concept equality_comparable_with =
    equality_comparable<T> && equality_comparable<U> &&
    common_reference_with<const std::remove_reference_t<T>&,
                          const std::remove_reference_t<U>&> &&
    equality_comparable<
        common_reference_t<const std::remove_reference_t<T>&,
                           const std::remove_reference_t<U>&> > &&
    __weakly_equality_comparable_with<T, U>;

/// The concept std::totally_ordered specifies that the comparison operators
/// ==,!=,<,>,<=,>= on a type yield results consistent with a strict total order
/// on the type.
///
/// See: https://en.cppreference.com/w/cpp/concepts/totally_ordered
template <class T>
concept totally_ordered =
    equality_comparable<T> && requires(const std::remove_reference_t<T>& a,
                                       const std::remove_reference_t<T>& b) {
      { a < b } -> boolean_testable;
      { a > b } -> boolean_testable;
      { a <= b } -> boolean_testable;
      { a >= b } -> boolean_testable;
    };

/// The concept std::totally_ordered_with specifies that the comparison
/// operators ==,!=,<,>,<=,>= on (possibly mixed) T and U operands yield results
/// consistent with a strict total order. Comparing mixed operands yields
/// results equivalent to comparing the operands converted to their common type.
///
/// See: https://en.cppreference.com/w/cpp/concepts/totally_ordered
template <class T, class U>
concept totally_ordered_with =
    totally_ordered<T> && totally_ordered<U> &&
    common_reference_with<const std::remove_reference_t<T>&,
                          const std::remove_reference_t<U>&> &&
    totally_ordered<common_reference_t<const std::remove_reference_t<T>&,
                                       const std::remove_reference_t<U>&> > &&
    equality_comparable_with<T, U> &&
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u) {
      { t < u } -> boolean_testable;
      { t > u } -> boolean_testable;
      { t <= u } -> boolean_testable;
      { t >= u } -> boolean_testable;
      { u < t } -> boolean_testable;
      { u > t } -> boolean_testable;
      { u <= t } -> boolean_testable;
      { u >= t } -> boolean_testable;
    };

/// The concept movable<T> specifies that T is an object type that can be moved
/// (that is, it can be move constructed, move assigned, and lvalues of type T
/// can be swapped).
///
/// See: https://en.cppreference.com/w/cpp/concepts/movable
template <class T>
concept movable = std::is_object_v<T> && move_constructible<T> &&
                  assignable_from<T&, T> && swappable<T>;

/// The concept copyable<T> specifies that T is a movable object type that can
/// also copied (that is, it supports copy construction and copy assignment).
///
/// See: https://en.cppreference.com/w/cpp/concepts/copyable
template <class T>
concept copyable =
    copy_constructible<T> && movable<T> && assignable_from<T&, T&> &&
    assignable_from<T&, const T&> && assignable_from<T&, const T>;

/// The semiregular concept specifies that a type is both copyable and default
/// constructible. It is satisfied by types that behave similarly to built-in
/// types like int, except that they need not support comparison with ==.
///
/// See: https://en.cppreference.com/w/cpp/concepts/semiregular
template <class T>
concept semiregular = copyable<T> && default_initializable<T>;

/// The regular concept specifies that a type is regular, that is, it is
/// copyable, default constructible, and equality comparable. It is satisfied by
/// types that behave similarly to built-in types like int, and that are
/// comparable with ==.
///
/// See: https://en.cppreference.com/w/cpp/concepts/regular
template <class T>
concept regular = semiregular<T> && equality_comparable<T>;

/// The invocable concept specifies that a callable type F can be called with a
/// set of arguments Args... using the function template std::invoke.
///
/// See: https://en.cppreference.com/w/cpp/concepts/invocable
template <class F, class... Args>
concept invocable = requires(F&& f, Args&&... args) {
  std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
};

/// The regular_invocable concept adds to the invocable concept by requiring the
/// invoke expression to be equality-preserving and not modify either the
/// function object or the arguments.
///
/// See: https://en.cppreference.com/w/cpp/concepts/invocable
template <class F, class... Args>
concept regular_invocable = invocable<F, Args...>;

/// The concept predicate<F, Args...> specifies that F is a predicate that
/// accepts arguments whose types and value categories are encoded by Args...,
/// i.e., it can be invoked with these arguments to produce a boolean-testable
/// result.
///
/// See: https://en.cppreference.com/w/cpp/concepts/predicate
template <class F, class... Args>
concept predicate = regular_invocable<F, Args...> &&
                    boolean_testable<std::invoke_result_t<F, Args...> >;

/// The concept relation<R, T, U> specifies that R defines a binary relation
/// over the set of expressions whose type and value category are those encoded
/// by either T or U.
///
/// See: https://en.cppreference.com/w/cpp/concepts/relation
template <class R, class T, class U>
concept relation = predicate<R, T, T> && predicate<R, U, U> &&
                   predicate<R, T, U> && predicate<R, U, T>;

/// The concept equivalence_relation<R, T, U> specifies that the relation R
/// imposes an equivalence relation on its arguments.
///
/// See: https://en.cppreference.com/w/cpp/concepts/equivalence_relation
template <class R, class T, class U>
concept equivalence_relation = relation<R, T, U>;

/// The concept strict_weak_order<R, T, U> specifies that the relation R imposes
/// a strict weak ordering on its arguments.
///
/// See: https://en.cppreference.com/w/cpp/concepts/strict_weak_order
template <class R, class T, class U>
concept strict_weak_order = relation<R, T, U>;

#endif  // YAT_HAS_STDLIB_FEATURE(concepts, 202002L)

}  // namespace YAT_STDCPP20 cpp20
#pragma endregion  // C++20 Library Features

}  // namespace v2
}  // namespace yat
// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <type_traits>

#include "__compat.hpp"
#include "__config.hpp"

namespace yat {
inline namespace v2 {

#pragma region copy_cv

/// The class template copy_cv copies the const/volatile qualifiers from one
/// type to another.
template <typename From, typename To>
struct copy_cv {
  using type = To;
};

template <typename From, typename To>
struct copy_cv<From const, To> {
  using type = std::add_const_t<To>;
};

template <typename From, typename To>
struct copy_cv<From volatile, To> {
  using type = std::add_volatile_t<To>;
};

template <typename From, typename To>
struct copy_cv<From const volatile, To> {
  using type = std::add_cv_t<To>;
};

/// The class template copy_cv copies the const/volatile qualifiers from one
/// type to another.
template <typename From, typename To>
using copy_cv_t = typename copy_cv<From, To>::type;

#pragma endregion  // copy_cv
#pragma region copy_cvref

/// The class template copy_cvref copies the const/volatile qualifiers and
/// reference category from one type to another.
template <typename From, typename To>
struct copy_cvref {
  using type = copy_cv_t<From, To>;
};

template <typename From, typename To>
struct copy_cvref<From const, To> {
  using type = std::add_const_t<copy_cv_t<From, To>>;
};

template <typename From, typename To>
struct copy_cvref<From volatile, To> {
  using type = std::add_volatile_t<copy_cv_t<From, To>>;
};

template <typename From, typename To>
struct copy_cvref<From const volatile, To> {
  using type = std::add_cv_t<copy_cv_t<From, To>>;
};

/// The class template copy_cvref copies the const/volatile qualifiers and
/// reference category from one type to another.
template <typename From, typename To>
using copy_cvref_t = typename copy_cvref<From, To>::type;

#pragma endregion  // copy_cvref
#pragma region is_specialization_of

/// Determines if a type specializes a templated type
template <typename T, template <typename...> typename TT>
struct is_specialization_of : std::false_type {};

/// Determines if a type specializes a templated type
template <template <typename...> typename T, typename... Args>
struct is_specialization_of<T<Args...>, T> : std::true_type {};

/// Determines if a type specializes a templated type
template <typename T, template <typename...> typename TT>
using is_specialization_of_t = typename is_specialization_of<T, TT>::type;

/// Determines if a type specializes a templated type
template <typename T, template <typename...> typename TT>
inline constexpr bool is_specialization_of_v =
    is_specialization_of<T, TT>::value;

#pragma endregion  // is_specialization_of

#pragma region C++20 Library Features
inline namespace YAT_STDCPP20 cpp20 {

#pragma region std::common_reference
#if YAT_HAS_STDLIB_FEATURE(concepts, 202002L)

using std::basic_common_reference;
using std::common_reference;
using std::common_reference_t;

#else
inline namespace __common_reference {

/// The class template basic_common_reference is a customization point that
/// allows users to influence the result of common_reference for user-defined
/// types (typically proxy references). The primary template is empty.
///
/// See: https://en.cppreference.com/w/cpp/types/common_reference
template <class T, class U, template <class> class TQual,
          template <class> class UQual>
struct basic_common_reference {};

/// Determines the common reference type of the types T..., that is, the type to
/// which all the types in T... can be converted or bound.
///
/// See: https://en.cppreference.com/w/cpp/types/common_reference
template <class... T>
struct common_reference;

/// Determines the common reference type of the types T..., that is, the type to
/// which all the types in T... can be converted or bound.
///
/// See: https://en.cppreference.com/w/cpp/types/common_reference
template <class... T>
using common_reference_t = typename std::common_reference<T...>::type;

template <typename A>
using __lvref = std::add_lvalue_reference_t<std::remove_reference_t<A>>;

template <typename A>
using __rvref = std::add_rvalue_reference_t<std::remove_reference_t<A>>;

//
// Let CREF(A) be add_lvalue_reference_t<const remove_reference_t<A>>.
//
template <typename A>
using __cref =
    std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<A>>>;

//
// Let XREF(A) denote a unary alias template T such that T<U> denotes the same
// type as U with the addition of A's cv and reference qualifiers, for a
// non-reference cv-unqualified type U.
//
template <typename A>
struct __xref {
  template <typename U>
  using T = copy_cvref_t<A, U>;
};

//
// Let COPYCV(FROM, TO) be an alias for type TO with the addition of FROM's
// top-level cv-qualifiers.
//
template <typename From, typename To>
using __copycv = copy_cv_t<From, To>;

//
// COND-RES(X, Y) be decltype(false ? declval<X(&)()>()() :
// declval<Y(&)()>()()).
//
template <typename X, typename Y>
using __cond_res =
    decltype(false ? std::declval<X (&)()>()() : std::declval<Y (&)()>()());

//
// Given types A and B, let X be remove_reference_t<A>,
// let Y be remove_reference_t<B>, and let COMMON-​REF(A, B) be:
//
template <typename A, typename B, typename X = std::remove_reference_t<A>,
          typename Y = std::remove_reference_t<B>>
struct __common_ref {};

template <typename A, typename B>
using __common_ref_t = typename __common_ref<A, B>::type;

//
// If A and B are both lvalue reference types, COMMON-REF(A, B) is
// COND-RES(COPYCV(X, Y) &, COPYCV(​Y, X) &) if that type exists and is a
// reference type.
//
template <typename A, typename B, typename X, typename Y>
  requires requires {
    typename __cond_res<__copycv<X, Y> &, __copycv<Y, X> &>;
  } && std::is_reference_v<__cond_res<__copycv<X, Y> &, __copycv<Y, X> &>>
struct __common_ref<A &, B &, X, Y> {
  using type = __cond_res<__copycv<X, Y> &, __copycv<Y, X> &>;
};

//
// Otherwise, let C be remove_reference_t<COMMON-REF(X&, Y&)>&&. If A and B are
// both rvalue reference types, C is well-formed, and is_convertible_v<A, C> &&
// is_convertible_v<B, C> is true, then COMMON-REF(A, B) is C.
//
template <typename A, typename B, typename X, typename Y>
  requires requires { typename __rvref<__common_ref_t<X &, Y &>>; } &&
           std::is_convertible_v<A, __rvref<__common_ref_t<X &, Y &>>> &&
           std::is_convertible_v<B, __rvref<__common_ref_t<X &, Y &>>>
struct __common_ref<A &&, B &&, X, Y> {
  using type = __rvref<__common_ref_t<X &, Y &>>;
};

//
// Otherwise, let D be COMMON-REF(const X&, Y&). If A is an rvalue reference and
// B is an lvalue reference and D is well-formed and is_convertible_v<A, D> is
// true, then COMMON-REF(A, B) is D.
//
template <typename A, typename B, typename X, typename Y>
  requires requires { typename __common_ref_t<const X &, Y &>; } &&
           std::is_convertible_v<A &&, __common_ref_t<const X &, Y &>>
struct __common_ref<A &&, B &, X, Y> {
  using type = __common_ref_t<const X &, Y &>;
};

//
// Otherwise, if A is an lvalue reference and B is an rvalue reference, then
// COMMON-REF(A, B) is COMMON-REF(B, A).
//
template <typename A, typename B, typename X, typename Y>
struct __common_ref<A &, B &&, X, Y> : __common_ref<B &&, A &, Y, X> {};

//
// If sizeof...(T) is zero, there shall be no member type.
//
template <>
struct common_reference<> {};

//
// Otherwise, if sizeof...(T) is one, let T0 denote the sole type in the pack T.
// The member typedef type shall denote the same type as T0.
//
template <class T0>
struct common_reference<T0> {
  using type = T0;
};

//
// Otherwise, if sizeof...(T) is two, let T1 and T2 denote the two types in the
// pack T. Then
//
template <class T1, class T2>
struct common_reference<T1, T2>;

//
// Let R be COMMON-REF(T1, T2). If T1 and T2 are reference types, R is
// well-formed, and is_convertible_v<add_pointer_t<T1>, add_pointer_t<R>> &&
// is_convertible_v<add_pointer_t<T2>, add_pointer_t<R>> is true, then the
// member typedef type denotes R.
//
template <class T1, class T2>
  requires std::is_reference_v<T1> && std::is_reference_v<T2> &&
           requires { typename __common_ref_t<T1, T2>; } &&
           std::is_convertible_v<std::add_pointer_t<T1>,
                                 std::add_pointer_t<__common_ref_t<T1, T2>>> &&
           std::is_convertible_v<std::add_pointer_t<T2>,
                                 std::add_pointer_t<__common_ref_t<T1, T2>>>
struct common_reference<T1, T2> {
  using type = __common_ref_t<T1, T2>;
};

//
// Otherwise, if basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>,
// XREF(T1), XREF(T2)>::type is well-formed, then the member typedef type
// denotes that type.
//
template <class T1, class T2>
  requires requires {
    typename basic_common_reference<std::remove_cvref_t<T1>,
                                    std::remove_cvref_t<T2>, __xref<T1>::T,
                                    __xref<T2>::T>;
  }
struct common_reference<T1, T2>
    : basic_common_reference<std::remove_cvref_t<T1>, std::remove_cvref_t<T2>,
                             __xref<T1>::T, __xref<T2>::T> {};

//
// Otherwise, if COND-RES(T1, T2) is well-formed, then the member typedef type
// denotes that type
//
template <class T1, class T2>
  requires requires { typename __cond_res<T1, T2>; }
struct common_reference<T1, T2> {
  using type = __cond_res<T1, T2>;
};

//
// Otherwise, if common_type_t<T1, T2> is well-formed, then the member typedef
// type denotes that type.
//
template <class T1, class T2>
  requires requires { typename std::common_type_t<T1, T2>; }
struct common_reference<T1, T2> {
  using type = std::common_type_t<T1, T2>;
};

//
// Otherwise, there shall be no member type.
//
template <class T1, class T2>
struct common_reference<T1, T2> {};

//
// Otherwise, if sizeof...(T) is greater than two, let T1, T2, and Rest,
// respectively, denote the first, second, and (pack of) remaining types
// comprising T. Let C be the type common_reference_t<T1, T2>. Then:
//
template <class T1, class T2, class T3, class... Rest>
  requires(sizeof...(Rest) > 0)
struct common_reference<T1, T2, Rest...>;

//
// If there is such a type C, the member typedef type shall denote the same
// type, if any, as common_reference_t<C, Rest...>
//
template <class T1, class T2, class... Rest>
  requires(sizeof...(Rest) > 0) &&
          requires { typename common_reference_t<T1, T2>; }
struct common_reference<T1, T2, Rest...> {
  using type = common_reference_t<common_reference_t<T1, T2>, Rest...>;
};

//
// Otherwise, there shall be no member type.
//
template <class T1, class T2, class... Rest>
  requires(sizeof...(Rest) > 0)
struct common_reference<T1, T2, Rest...> {};

}  // namespace __common_reference
#endif  // YAT_HAS_STDLIB_FEATURE(concepts, 202002L)
#pragma endregion std::common_reference
#pragma region std::is_[un]bounded_array
#if YAT_HAS_STDLIB_FEATURE(bounded_array_traits, 201902L)

using std::is_bounded_array;
using std::is_bounded_array_v;
using std::is_unbounded_array;
using std::is_unbounded_array_v;

#else

/// Checks whether T is an array type of known bound. Provides the
/// member constant value which is equal to true, if T is an array
/// type of known bound. Otherwise, value is equal to false.
///
/// The behavior of a program that adds specializations for
/// is_bounded_array or is_bounded_array_v is undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/is_bounded_array
template <class T>
struct is_bounded_array : std::false_type {};

template <class T, std::size_t N>
struct is_bounded_array<T[N]> : std::true_type {};

/// Checks whether T is an array type of known bound. Provides the member
/// constant value which is equal to true, if T is an array type of known bound.
/// Otherwise, value is equal to false.
///
/// The behavior of a program that adds specializations for is_bounded_array or
/// is_bounded_array_v is undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/is_bounded_array
template <class T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

/// Checks whether T is an array type of unknown bound. Provides the member
/// constant value which is equal to true, if T is an array type of unknown
/// bound. Otherwise, value is equal to false.
///
/// The behavior of a program that adds specializations for is_unbounded_array
/// or is_unbounded_array_v is undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/is_unbounded_array
template <class T>
struct is_unbounded_array : std::false_type {};

template <class T>
struct is_unbounded_array<T[]> : std::true_type {};

/// Checks whether T is an array type of unknown bound. Provides the member
/// constant value which is equal to true, if T is an array type of unknown
/// bound. Otherwise, value is equal to false.
///
/// The behavior of a program that adds specializations for is_unbounded_array
/// or is_unbounded_array_v is undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/is_unbounded_array
template <class T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

#endif             // YAT_HAS_STDLIB_FEATURE(bounded_array_traits, 201902L)
#pragma endregion  // std::is_[un]bounded_array
#pragma region std::is_nothrow_convertible
#if YAT_HAS_STDLIB_FEATURE(is_nothrow_convertible, 201806L)

using std::is_nothrow_convertible;
using std::is_nothrow_convertible_v;

#else

/// Same as is_convertible, except that it checks whether the conversion is
/// noexcept.
///
/// See: https://en.cppreference.com/w/cpp/types/is_convertible
template <class From, class To>
struct is_nothrow_convertible : std::false_type {};

template <class From, class To>
  requires std::is_convertible_v<From, To> && requires {
    { static_cast<To>(std::declval<From>()) } noexcept;
  }
struct is_nothrow_convertible<From, To> : std::true_type {};

/// Same as is_convertible_v, except that it checks whether the conversion is
/// noexcept.
///
/// See: https://en.cppreference.com/w/cpp/types/is_convertible
template <class From, class To>
inline constexpr bool is_nothrow_convertible_v =
    is_nothrow_convertible<From, To>::value;

#endif             // YAT_HAS_STDLIB_FEATURE(is_nothrow_convertible, 201806L)
#pragma endregion  // std::is_nothrow_convertible
#pragma region std::remove_cvref
#if YAT_HAS_STDLIB_FEATURE(remove_cvref, 201711L)

using std::remove_cvref;
using std::remove_cvref_t;

#else

/// If the type T is a reference type, provides the member typedef type which is
/// the type referred to by T with its topmost cv-qualifiers removed. Otherwise
/// type is T with its topmost cv-qualifiers removed.
///
/// The behavior of a program that adds specializations for remove_cvref is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/remove_cvref
template <class T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

/// If the type T is a reference type, provides the member typedef type which is
/// the type referred to by T with its topmost cv-qualifiers removed. Otherwise
/// type is T with its topmost cv-qualifiers removed.
///
/// The behavior of a program that adds specializations for remove_cvref is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/remove_cvref
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

#endif             // YAT_HAS_STDLIB_FEATURE(remove_cvref, 201711L)
#pragma endregion  // std::remove_cvref
#pragma region std::type_identity
#if YAT_HAS_STDLIB_FEATURE(type_identity, 201806L)

using std::type_identity;
using std::type_identity_t;

#else

/// Provides the member typedef type which is identical to T, except that its
/// topmost cv-qualifiers are removed, unless B is true.
///
/// The behavior of a program that adds specializations for type_identity is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/type_identity
template <class T>
struct type_identity {
  using type = T;
};

/// Provides the member typedef type which is identical to T, except that its
/// topmost cv-qualifiers are removed, unless B is true.
///
/// The behavior of a program that adds specializations for type_identity is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/type_identity
template <class T>
using type_identity_t = typename type_identity<T, B>::type;

#endif             // YAT_HAS_STDLIB_FEATURE(type_identity, 201806L)
#pragma endregion  // std::type_identity
#pragma region std::unwrap_reference
#if YAT_HAS_STDLIB_FEATURE(unwrap_ref, 201811L)

using std::unwrap_ref_decay;
using std::unwrap_ref_decay_t;
using std::unwrap_reference;
using std::unwrap_reference_t;

#else

/// If T is std::reference_wrapper<U> for some type U, provides a member type
/// alias type that names U&; otherwise, provides a member type alias type that
/// names T.
///
/// The behavior of a program that adds specializations for unwrap_reference is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
template <class T>
struct unwrap_reference {
  using type = T;
};

template <class T>
struct unwrap_reference<std::reference_wrapper<T>> {
  using type = T &;
};

/// If T is std::reference_wrapper<U> for some type U, provides a member type
/// alias type that names U&; otherwise, provides a member type alias type that
/// names T.
///
/// The behavior of a program that adds specializations for unwrap_reference is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
template <class T>
using unwrap_reference_t = typename unwrap_reference<T>::type;

/// If T is std::reference_wrapper<U> for some type U, ignoring cv-qualification
/// and referenceness, provides a member type alias type that names U&;
/// otherwise, provides a member type alias type that names std::decay_t<T>.
///
/// The behavior of a program that adds specializations for unwrap_ref_decay is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
template <class T>
struct unwrap_ref_decay : unwrap_reference<std::decay_t<T>> {};

/// If T is std::reference_wrapper<U> for some type U, ignoring cv-qualification
/// and referenceness, provides a member type alias type that names U&;
/// otherwise, provides a member type alias type that names std::decay_t<T>.
///
/// The behavior of a program that adds specializations for unwrap_ref_decay is
/// undefined.
///
/// See: https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
template <class T>
using unwrap_ref_decay_t = typename unwrap_ref_decay<T>::type;

#endif             // YAT_HAS_STDLIB_FEATURE(unwrap_reference, 201811L)
#pragma endregion  // std::unwrap_reference

//
// NOTE: The following all require compiler support, so implementing them likely
// has no benefit, since compiler support likely means they are already defined
// in the standard library.
//

// TODO: std::is_layout_compatible
// TODO: std::is_corresponding_member
// TODO: std::is_pointer_interconvertible_base_of
// TODO: std::is_pointer_interconvertible_with_class
// TODO: std::is_constant_evaluated

}  // namespace YAT_STDCPP20 cpp20
#pragma endregion  // C++20 Library Features

#pragma region C++23 Library Features
inline namespace YAT_STDCPP23 cpp23 {

#pragma region std::is_scoped_enum
#if YAT_HAS_STDLIB_FEATURE(is_scoped_enum, 202011L)

using std::is_scoped_enum;
using std::is_scoped_enum_v;

#else

///
/// Checks whether T is a scoped enumeration type. Provides the member constant
/// value which is equal to true, if T is a scoped enumeration type. Otherwise,
/// value is equal to false.
///
/// The behavior of a program that adds specializations for is_scoped_enum or
/// is_scoped_enum_v is undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/is_scoped_enum
///
template <class T>
struct is_scoped_enum
    : std::bool_constant<std::is_enum_v<T> &&
                         !std::is_convertible_v<T, underlying_type_t<T>>> {};

///
/// Checks whether T is a scoped enumeration type. Provides the member constant
/// value which is equal to true, if T is a scoped enumeration type. Otherwise,
/// value is equal to false.
///
/// The behavior of a program that adds specializations for is_scoped_enum or
/// is_scoped_enum_v is undefined.
///
/// See: https://en.cppreference.com/w/cpp/types/is_scoped_enum
///
template <class T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

#endif             // YAT_HAS_STDLIB_FEATURE(is_scoped_enum, 202011L)
#pragma endregion  // std::is_scoped_enum

// TODO: std::basic_common_reference<std::pair>
// TODO: std::basic_common_reference<tuple-like>

//
// NOTE: The following requires compiler support, so implementing it likely has
// no benefit, since compiler support likely means they are already defined in
// the standard library.
//

// TODO: std::is_implicit_lifetime

}  // namespace YAT_STDCPP23 cpp23
#pragma endregion  // C++23 Library Features

}  // namespace v2
}  // namespace yat
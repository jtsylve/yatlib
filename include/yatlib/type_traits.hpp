/*
 * Copyright 2020-2021 Joe T. Sylve, Ph.D.
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
#pragma once

#include <cstddef>
#include <type_traits>
#include <version>

#include "features.hpp"
#include "variant.hpp"

/////////////////////////////////////////////////////////////
// Determine if a given type is a member of a std::variant //
/////////////////////////////////////////////////////////////

namespace yat {

/// Determines if a given type is a alternative type of a given variant.
template <typename T, typename VariantType>
struct is_variant_alternative;

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename... Types>
struct is_variant_alternative<T, yat::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...> {};

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename VariantType>
using is_variant_alternative_t =
    typename is_variant_alternative<T, VariantType>::type;

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename VariantType>
inline constexpr bool is_variant_alternative_v =
    is_variant_alternative<T, VariantType>::value;

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
struct is_one_of : std::disjunction<std::is_same<T, Types>...> {};

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
using is_one_of_t = typename is_one_of<T, Types...>::type;

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
inline constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

/// Determines if T is a character type
template <typename T>
struct is_char_type : is_one_of<std::decay_t<T>, char, signed char,
                                unsigned char, wchar_t, char16_t, char32_t> {};

#ifdef YAT_SUPPORTS_CPP20
template <>
struct is_char_type<char8_t> : std::true_type {};
#endif  // YAT_SUPPORTS_CPP20

/// Determines if T is a character type
template <typename T>
inline constexpr bool is_char_type_v = is_char_type<T>::value;

/// Determines if T is a character type
template <typename T>
using is_char_type_t = typename is_char_type<T>::type;

/// Determines if T is dereferencable
template <typename T, typename = void>
struct is_dereferencable : std::false_type {};

/// Determines if T is dereferencable
template <typename T>
struct is_dereferencable<T, std::void_t<decltype(*std::declval<T>())>>
    : std::true_type {};

/// Determines if T is dereferencable
template <typename T>
using is_dereferencable_t = typename is_dereferencable<T>::type;

/// Determines if T is dereferencable
template <typename T>
inline constexpr bool is_dereferencable_v = is_dereferencable<T>::value;

/// Determines if an array of T is convertible to an array of U
template <typename T, typename U>
struct is_array_convertible : std::is_convertible<T (*)[], U (*)[]> {};

/// Determines if an array of T is convertible to an array of U
template <typename T, typename U>
using is_array_convertible_t = typename is_array_convertible<T, U>::type;

/// Determines if an array of T is convertible to an array of U
template <typename T, typename U>
inline constexpr bool is_array_convertible_v =
    is_array_convertible<T, U>::value;

}  // namespace yat

/////////////////////////////////////////
// P0887R1 - https://wg21.link/P0887R1 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_type_identity) && __cpp_lib_type_identity >= 201806L
#define YAT_INTERNAL_USE_STD_TYPE_IDENTITY
#endif

#ifdef YAT_INTERNAL_USE_STD_TYPE_IDENTITY

namespace yat {

using std::type_identity;
using std::type_identity_t;

}  // namespace yat

#else

namespace yat {

/// Provides the member typedef type that names T (i.e., the identity
/// transformation).
///
/// The behavior of a program that adds specializations for type_identity is
/// undefined.
template <typename T>
struct type_identity {
  using type = T;
};

/// Provides the member typedef type that names T (i.e., the identity
/// transformation).
///
/// The behavior of a program that adds specializations for type_identity is
/// undefined.
template <typename T>
using type_identity_t = typename type_identity<T>::type;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_TYPE_IDENTITY

/////////////////////////////////////////
// P1048R1 - https://wg21.link/P1048R1 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
#define YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM
#endif

#ifdef YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM

namespace yat {

using std::is_scoped_enum;
using std::is_scoped_enum_v;

}  // namespace yat

#else

namespace yat::detail {
//
// Helper types for is_scoped_enum
//

template <typename T, bool = std::is_enum_v<T>>
struct is_scoped_enum_helper : std::false_type {};

template <typename T>
struct is_scoped_enum_helper<T, true>
    : std::negation<std::is_convertible<T, std::underlying_type_t<T>>> {};

}  // namespace yat::detail

namespace yat {

/// Checks whether T is an scoped enumeration type. Provides the member constant
/// value which is equal to true, if T is an scoped enumeration type. Otherwise,
/// value is equal to false.
template <typename T>
struct is_scoped_enum : detail::is_scoped_enum_helper<T> {};

/// true if T is an scoped enumeration type, false otherwise
template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM

/////////////////////////////////////////
// P0550R2 - https://wg21.link/P0550R2 //
/////////////////////////////////////////

#if defined(__cpp_lib_remove_cvref) && __cpp_lib_remove_cvref >= 201711
#define YAT_INTERNAL_USE_STD_REMOVE_CVREF
#endif

#ifdef YAT_INTERNAL_USE_STD_REMOVE_CVREF

namespace yat {

using std::remove_cvref;
using std::remove_cvref_t;

}  // namespace yat

#else

namespace yat {

/// If the type T is a reference type, provides the member typedef type which is
/// the type referred to by T with its topmost cv-qualifiers removed. Otherwise
/// type is T with its topmost cv-qualifiers removed.
///
/// The behavior of a program that adds specializations for remove_cvref is
/// undefined.
template <typename T>
struct remove_cvref {
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

/// Gives the type of a given `yat::remove_cvref` result
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_REMOVE_CVREF

/////////////////////////////////////////
// P0898R3 - https://wg21.link/P0898R3 //
/////////////////////////////////////////

#if defined(__cpp_lib_concepts) && __cpp_lib_concepts >= 202002
#define YAT_INTERNAL_USE_STD_COMMON_REFERENCE
#endif

#ifdef YAT_INTERNAL_USE_STD_COMMON_REFERENCE

namespace yat {

using std::basic_common_reference;
using std::common_reference;
using std::common_reference_t;

}  // namespace yat

#else

namespace yat {

/// The class template basic_common_reference is a customization point that
/// allows users to influence the result of common_reference for user-defined
/// types (typically proxy references). The primary template is empty.
template <typename, typename, template <typename> typename,
          template <typename> typename>
struct basic_common_reference {};

/// Determines the common reference type of the types T..., that is, the type to
/// which all the types in T... can be converted or bound. If such a type exists
/// (as determined according to the rules below), the member type names that
/// type. Otherwise, there is no member type. The behavior is undefined if any
/// of the types in T... is an incomplete type other than (possibly
/// cv-qualified) void
template <typename...>
struct common_reference;

/// The common reference type for all T...
template <class... _Types>
using common_reference_t = typename common_reference<_Types...>::type;

// N4810 [meta.trans.other]/5.1: "If sizeof...(T) is zero ..."
template <>
struct common_reference<> {};

// N4810 [meta.trans.other]/5.2: "...if sizeof...(T) is one ..."
template <typename T>
struct common_reference<T> {
  using type = T;
};

}  // namespace yat

namespace yat::detail {
//
// Helper types for common_reference
//

/// [meta.trans.other] 3.3
/// COPYCV(FROM, TO) be an alias for type TO with the addition of FROM’s
/// top-level cv-qualifiers [Example: COPYCV(const int, volatile short) is an
/// alias for const volatile short. —end example]
template <typename FROM, typename TO>
struct _copy_cv {
  using type = TO;
};

template <typename FROM, typename TO>
struct _copy_cv<const FROM, TO> {
  using type = const TO;
};

template <typename FROM, typename TO>
struct _copy_cv<volatile FROM, TO> {
  using type = volatile TO;
};

template <typename FROM, typename TO>
struct _copy_cv<const volatile FROM, TO> {
  using type = const volatile TO;
};

template <typename FROM, typename TO>
using copy_cv = typename _copy_cv<FROM, TO>::type;

/// [meta.trans.other] 3.2
/// XREF(A) denote a unary alias template T such that T<U> denotes the same type
/// as U with the addition of A’s cv and reference qualifiers, for a
/// non-reference cv-unqualified type U
template <typename T>
struct xref {
  template <typename U>
  using type = copy_cv<T, U>;
};

template <typename T>
struct xref<T&> {
  template <typename U>
  using type = std::add_lvalue_reference_t<copy_cv<T, U>>;
};

template <typename T>
struct xref<T&&> {
  template <typename U>
  using type = std::add_rvalue_reference_t<copy_cv<T, U>>;
};

/// [meta.trans.other] 3.4
/// COND-RES(X, Y) be decltype(false ? declval<X(&)()>()() :
/// declval<Y(&)()>()())
template <typename X, typename Y>
using cond_res =
    decltype(false ? std::declval<X (&)()>()() : std::declval<Y (&)()>()());

// N4810 [meta.trans.other]/5.3: "...if sizeof...(T) is two..."

// N4810 [meta.trans.other]/5.3.4: "if common_type_t<T1, T2> is well-formed..."
// N4810 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
template <typename T1, typename T2, typename = void>
struct __common_reference : std::common_type<T1, T2> {};

// N4810 [meta.trans.other]/5.3.3: "if COND_RES(T1, T2) is well-formed..."
template <typename T1, typename T2>
struct __common_reference<T1, T2, std::void_t<cond_res<T1, T2>>> {
  using type = cond_res<T1, T2>;
};

// N4810 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is
// well-formed..."
template <typename T1, typename T2>
using basic_specialization =
    typename yat::basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>,
                                         xref<T1>::template type,
                                         xref<T2>::template type>::type;

template <typename T1, typename T2, typename = void>
struct _common_reference : __common_reference<T1, T2> {};

template <typename T1, typename T2>
struct _common_reference<T1, T2, std::void_t<basic_specialization<T1, T2>>> {
  using type = basic_specialization<T1, T2>;
};

// N4810 [meta.trans.other]/5.3.1: "If T1 and T2 are reference types and
// COMMON_REF(T1, T2) is well-formed..."
template <typename T1, typename T2, typename = void>
struct common_reference : _common_reference<T1, T2> {};

template <typename T1, typename T2,
          typename Result = cond_res<copy_cv<T1, T2>&, copy_cv<T2, T1>&>,
          std::enable_if_t<std::is_lvalue_reference_v<Result>, int> = 0>
using ll_common_ref = Result;

template <typename T1, typename T2>
struct common_reference<T1&, T2&, std::void_t<ll_common_ref<T1, T2>>> {
  using type = ll_common_ref<T1, T2>;  // "both lvalues" case from N4810
                                       // [meta.trans.other]/2.5
};

template <typename T1, typename T2>
struct common_reference<T1&&, T2&,
                        std::enable_if_t<std::is_convertible_v<
                            T1&&, ll_common_ref<const T1, T2>>>> {
  using type = ll_common_ref<const T1, T2>;  // "rvalue and lvalue" case from
                                             // N4810 [meta.trans.other]/2.7
};

template <typename T1, typename T2>
struct common_reference<T1&, T2&&,
                        std::enable_if_t<std::is_convertible_v<
                            T2&&, ll_common_ref<const T2, T1>>>> {
  using type = ll_common_ref<const T2, T1>;  // "lvalue and rvalue" case from
                                             // N4810 [meta.trans.other]/2.8
};

template <typename T1, typename T2>
using rr_common_ref = std::remove_reference_t<ll_common_ref<T1, T2>>&&;

template <typename T1, typename T2>
struct common_reference<
    T1&&, T2&&,
    std::enable_if_t<std::is_convertible_v<T1&&, rr_common_ref<T1, T2>> &&
                     std::is_convertible_v<T2&&, rr_common_ref<T1, T2>>>> {
  using type = rr_common_ref<T1, T2>;  // "both rvalues" case from N4810
                                       // [meta.trans.other]/2.6
};

// N4810 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
template <typename Void, typename T1, typename T2, typename... Rest>
struct fold_common_reference {};

template <typename T1, typename T2, typename... Rest>
struct fold_common_reference<std::void_t<common_reference_t<T1, T2>>, T1, T2,
                             Rest...>
    : yat::common_reference<common_reference_t<T1, T2>, Rest...> {};

}  // namespace yat::detail

namespace yat {

template <typename T1, typename T2>
struct common_reference<T1, T2> : detail::common_reference<T1, T2> {};

template <typename T1, typename T2, typename T3, typename... Rest>
struct common_reference<T1, T2, T3, Rest...>
    : detail::fold_common_reference<void, T1, T2, T3, Rest...> {};

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_COMMON_REFERENCE

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_TYPE_IDENTITY
#undef YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM
#undef YAT_INTERNAL_USE_STD_REMOVE_CVREF
#undef YAT_INTERNAL_USE_STD_COMMON_REFERENCE
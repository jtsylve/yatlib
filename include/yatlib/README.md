# YATLib - Features

This document describes the features available in YATLib.  It is organized by header.  When `std` versions of implementations are available, implementations in the `yat` namespace are simply aliases to their `std` counterpart.

## any.hpp

Apple disallows the use of std::any before macOS 10.14 because the `std::bad_any_cast` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<any>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::any` on those systems.

All of the `std` types in the `<any>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<any>` header elsewhere, this may fail to work properly.

## array.hpp

```cpp
template <typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]);

template <typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T(&&a)[N]);
```

### yat::to_array

`yat::to_array` provides an implementation of [std::to_array](https://en.cppreference.com/w/cpp/container/array/to_array).

These functions creates a `std::array` from one dimensional built-in arrays.

## bit.hpp

```cpp
enum class endian
{
    little = /*implementation-defined*/,
    big    = /*implementation-defined*/,
    native = /*implementation-defined*/
};

template <typename To, typename From>
constexpr To bit_cast(const From& from) noexcept;

template <typename T>
constexpr T byteswap(T value) noexcept;

template<typename T>
[[nodiscard]] constexpr T rotl(T x, int s) noexcept;
  
template<typename T>
[[nodiscard]] constexpr T rotr(T x, int s) noexcept;

template<typename T>
constexpr int countl_zero(T x) noexcept;
  
template<typename T>
constexpr int countl_one(T x) noexcept;
  
template<typename T>
constexpr int countr_zero(T x) noexcept;
  
template<typename T>
constexpr int countr_one(T x) noexcept;
  
template<typename T>
constexpr int popcount(T x) noexcept;
```

### yat::endian

`yat::endian` provides an implementation of [std::endian](https://en.cppreference.com/w/cpp/types/endian).

### yat::bit_cast

`yat::bitcast provides an implementation of [std::bitcast](https://en.cppreference.com/w/cpp/numeric/bit_cast).

NOTE: On compilers without `__builtin_bitcast` support and no native `std::bitcast` library implementation, this function is not `constexpr` and requires the return type to be default constructible.

### yat::byteswap

`yat::byteswap` provides an implementation of the proposed `std::byteswap` from [P1272R3](https://wg21.link/P1272R2) that swaps the bytes of integral types.  It also defines a specialization to support swapping of enumeration types, which will be the subject of a [future proposal](https://twitter.com/slurpsmadrips/status/1237915884573741057).  

These functions return a endian-swapped value of select scalar types.

NOTE: On MSVC compilers, this function is not `constexpr`

### yat::rotl

`yat::rotl` provides an implementation of [std::rotl](https://en.cppreference.com/w/cpp/numeric/rotl).

### yat::rotr

`yat::rotr` provides an implementation of [std::rotr](https://en.cppreference.com/w/cpp/numeric/rotr).

### yat::countl_zero

`yat::countl_zero` provides an implementation of [std::countl_zero](https://en.cppreference.com/w/cpp/numeric/countl_zero).

### yat::countl_one

`yat::countl_one` provides an implementation of [std::countl_one](https://en.cppreference.com/w/cpp/numeric/countl_one).

### yat::countr_zero

`yat::countr_zero` provides an implementation of [std::countr_zero](https://en.cppreference.com/w/cpp/numeric/countr_zero).

### yat::countr_one

`yat::countr_one` provides an implementation of [std::countr_one](https://en.cppreference.com/w/cpp/numeric/countr_one).

### yat::popcount

`yat::popcount` provides an implementation of [std::popcount](https://en.cppreference.com/w/cpp/numeric/popcount).

### Macros

* `YAT_HAS_CONSTEXPR_BIT_CAST` is defined when `yat::bit_cast` is `constexpr`
* `YAT_HAS_CONSTEXPR_BYTESWAP` is defined when `yat::byteswap` is `constexpr`

## concepts.hpp

**These types are only available when compiling with C++20 or above with a compiler that supports concepts.**

```cpp
template <typename T, typename VariantType>
concept variant_alternative_type;

template <typename T>
concept trivially_copyable;

template <typename T>
concept char_type;

template <typename T>
concept indexable

template <typename T, typename U>
concept same_as;

template <typename Derived, typename Base>
concept derived_from;

template <typename From, typename To>
concept convertible_to;

template <typename T, typename U>
concept common_reference_with;

template <typename T>
concept integral;

template <typename T>
concept signed_integral;

template <typename T>
concept unsigned_integral;

template <typename T>
concept floating_point;

template<typename LHS, typename RHS>
concept assignable_from;

template<typename T, typename U>
concept swappable_with;

template <typename T>
concept swappable;

template <typename T>
concept destructible;

template <typename T, typename... Args>
concept constructible_from;

template <typename T>
concept default_initializable;

template <typename T>
concept move_constructible;

template <typename T>
concept copy_constructible;
```

### yat::variant_alternative_type

`yat::variant_alternative_type` specifies that a given type is an alternative type of a given variant type.

### yat::trivially_copyable 

`yat::trivially_copyable` specifies that a givent type is trivially copyable.

### yat::char_type 

`yat::char_type` specifies that a givent type is a character type.

### yat::indexable

`yat::indexable` specifies that a given type can be indexed using the subscript operator.

### yat::same_as

`yat::same_as` provides an implementation of [std::same_as](https://en.cppreference.com/w/cpp/concepts/same_as).


### yat::derived_from

`yat::derived_from` provides an implementation of [std::derived_from](https://en.cppreference.com/w/cpp/concepts/derived_from).

### yat::convertible_to

`yat::convertible_to` provides an implementation of [std::convertible_to](https://en.cppreference.com/w/cpp/concepts/convertible_to).

### yat::common_reference_with

`yat::common_reference_with` provides an implementation of [std::common_reference_with](https://en.cppreference.com/w/cpp/concepts/common_reference_with).

### yat::integral

`yat::integral` provides an implementation of [std::integral](https://en.cppreference.com/w/cpp/concepts/integral).

### yat::signed_integral

`yat::signed_integral` provides an implementation of [std::signed_integral](https://en.cppreference.com/w/cpp/concepts/signed_integral).

### yat::unsigned_integral

`yat::unsigned_integral` provides an implementation of [std::unsigned_integral](https://en.cppreference.com/w/cpp/concepts/unsigned_integral).

### yat::floating_point

`yat::floating_point` provides an implementation of [std::floating_point](https://en.cppreference.com/w/cpp/concepts/floating_point).

### yat::assignable_from

`yat::assignable_from` provides an implementation of [std::assignable_from](https://en.cppreference.com/w/cpp/concepts/assignable_from).

### yat::swappable

`yat::swappable` provides an implementation of [std::swappable](https://en.cppreference.com/w/cpp/concepts/swappable).

### yat::swappable_with

`yat::swappable_with` provides an implementation of [std::swappable_with](https://en.cppreference.com/w/cpp/concepts/swappable).

### yat::destructible

`yat::destructible` provides an implementation of [std::destructible](https://en.cppreference.com/w/cpp/concepts/destructible).

### yat::constructible_from

`yat::constructible_from` provides an implementation of [std::constructible_from](https://en.cppreference.com/w/cpp/concepts/contructible_from).

### yat::default_initializable

`yat::default_initializable` provides an implementation of [std::default_initializable](https://en.cppreference.com/w/cpp/concepts/default_initializable).

### yat::move_constructible

`yat::move_constructible` provides an implementation of [std::move_constructible](https://en.cppreference.com/w/cpp/concepts/move_constructible).

### yat::copy_constructible

`yat::copy_constructible` provides an implementation of [std::copy_constructible](https://en.cppreference.com/w/cpp/concepts/copy_constructible).


## endian.hpp

```cpp
inline constexpr bool is_little_endian_system;
inline constexpr bool is_big_endian_system;

template <typename T>
struct endian_byte_swapper;

template <typename T, yat::endian Endianess,
          typename ByteSwapper = endian_byte_swapper<T>>
class basic_endian_scalar;

template <typename T>
using big_scalar = basic_endian_scalar<T, endian::big>;

template <typename T>
using little_scalar = basic_endian_scalar<T, endian::little>;

using big_int8_t = big_scalar<int8_t>;
using big_uint8_t = big_scalar<uint8_t>;
using big_int16_t = big_scalar<int16_t>;
using big_uint16_t = big_scalar<uint16_t>;
using big_int32_t = big_scalar<int32_t>;
using big_uint32_t = big_scalar<uint32_t>;
using big_int64_t = big_scalar<int64_t>;
using big_uint64_t = big_scalar<uint64_t>;
using big_intptr_t = big_scalar<intptr_t>;
using big_uintptr_t = big_scalar<uintptr_t>;

using little_int8_t = little_scalar<int8_t>;
using little_uint8_t = little_scalar<uint8_t>;
using little_int16_t = little_scalar<int16_t>;
using little_uint16_t = little_scalar<uint16_t>;
using little_int32_t = little_scalar<int32_t>;
using little_uint32_t = little_scalar<uint32_t>;
using little_int64_t = little_scalar<int64_t>;
using little_uint64_t = little_scalar<uint64_t>;
using little_intptr_t = little_scalar<intptr_t>;
using little_uintptr_t = little_scalar<uintptr_t>;
```

### Constants

* `yat::is_little_endian_system` tells whether code is being compiled for a little endian system.
* `yat::is_big_endian_system` tells whether code is being compiled for a big endian system.

### Endian Scalar Types

* `yat::basic_endian_scalar` provides support for reading and writing possibly non-native endian types from/to disk or memory.  Currently these types do not support arithmetic operations, as misuse of these could cause performance issues.
* `yat::endian_byte_swapper` can be specialized so that custom types can be supported by `yat::basic_endian_scalar`.  The default implementation supports all types supported by `yat::byteswap`.

## filesystem.hpp

Apple disallows the use of std::filesystem before macOS 10.15 because the library support is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<filesystem>` provides a namespace alias to the [ghc::filesystem](https://github.com/gulrak/filesystem) implementation when std::filesystem support isn't available.

NOTE: If your project or its dependencies import the `<filesystem>` header elsewhere, this may fail to work properly.

In addition the following custom features are provided:

```cpp
filesystem::path copy_with_extension(const filesystem::path &path,
                                     const filesystem::path &ext);
```

### yat::copy_with_extension

`yat::copy_with_extension` creates a copy of the path, but with a new extension.

## iterator.hpp

```cpp
template <typename T>
struct incrementable_traits;

template <typename T, typename = std::enable_if_t<is_dereferencable_v<T> > >
using iter_reference_t = decltype(*std::declval<T&>());
```

### yat::incrementable_traits

`yat::incrementable_traits` provides an implementation of [std::incrementable_traits](https://en.cppreference.com/w/cpp/iterator/incrementable_traits).

It computes the associated difference type of the type `T`, if any. Users may specialize `yat::incrementable_traits` for a program-defined type.

### yat::iter_reference_t

`yat::iter_reference_t` provides an implementation of [std::iter_reference_t](https://en.cppreference.com/w/cpp/iterator/iter_t).  

It computes the reference type of T..

## memory.hpp

```cpp
template <typename Ptr>
constexpr auto to_address(const Ptr& p) noexcept;

template <typename T>
constexpr T* to_address(T* p) noexcept;

template <typename T>
class refcnt_ptr;

template <typename T, typename... Args>
inline refcnt_ptr<T> make_refcnt(Args&&... args);

template <typename T, typename U>
inline refcnt_ptr<T> static_pointer_cast(const refcnt_ptr<U>& r) noexcept;

template <typename T, typename U>
inline refcnt_ptr<T> dynamic_pointer_cast(const refcnt_ptr<U>& r) noexcept;

template <typename T, typename U>
inline refcnt_ptr<T> const_pointer_cast(const refcnt_ptr<U>& r) noexcept;

template <typename T, typename U>
inline refcnt_ptr<T> reinterpret_pointer_cast(const refcnt_ptr<U>& r) noexcept;

```

### yat::to_address

`yat::to_address` provides an implementation of [std::to_address](https://en.cppreference.com/w/cpp/memory/to_address).  

It obtains the address represented by p without forming a reference to the pointee.

### yat::refcnt_ptr

`yat::refcnt_ptr` provides a light-weight reference counted smart pointer that is similar to std::shared_ptr, but is not thread-safe and lacks many of the bells and whistles.

Specifically, it does not support the following:
* Thread-safe reference counting
* Custom Deleters
* Type-erased pointers
* Array pointers

There are also a number of support functions provided that match the functionality of the std::shared_ptr support functions:
* `yat::make_refcnt`
* `yat::static_pointer_cast`
* `yat::dynamic_pointer_cast`
* `yat::const_pointer_cast`
* `yat::reinterpret_pointer_cast`

## optional.hpp

Apple disallows the use of std::optional before macOS 10.14 because the `std::bad_optional_access` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<optional>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::optional` on those systems.

All of the `std` types in the `<optional>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<optional>` header elsewhere, this may fail to work properly.

## span.hpp

Importing this header instead of `<span>` provides an alias to the [GSL::span](https://github.com/microsoft/GSL/blob/main/include/gsl/span) implementation when std::span support isn't available.

```cpp
inline constexpr size_t dynamic_extent = numeric_limits<size_t>::max();

template<typename ElementType, size_t Extent = dynamic_extent>
typename span;
 
template<typename ElementType, size_t Extent>
span<const byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>
as_bytes(span<ElementType, Extent> s) noexcept;
 
template<typename ElementType, size_t Extent>
span<byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>
as_writable_bytes(span<ElementType, Extent> s) noexcept;
```

## type_traits.hpp

```cpp
template <typename T, typename VariantType>
struct is_variant_alternative;

template <typename T, typename VariantType>
using is_variant_alternative_t = typename is_variant_alternative<T, VariantType>::type;

template <typename T, typename VariantType>
inline constexpr bool is_variant_alternative_v = is_variant_alternative<T, VariantType>::value;

template <typename T, typename... Types>
struct is_one_of;

template <typename T, typename... Types>
using is_one_of_t = typename is_one_of<T, Types...>::type;

template <typename T, typename... Types>
inline constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

template <typename T>
struct is_char_type;

template <typename T>
inline constexpr bool is_char_type_v = is_char_type<T>::value;

template <typename T>
using is_char_type_t = typename is_char_type<T>::type;

template <typename T>
struct is_dereferencable;

template <typename T>
using is_dereferencable_t = typename is_dereferencable<T>::type;

template <typename T>
inline constexpr bool is_dereferencable_v = is_dereferencable<T>::value;

template <typename T, typename U>
struct is_array_convertible;

template <typename T, typename U>
using is_array_convertible_t = typename is_array_convertible<T, U>::type;

template <typename T, typename U>
inline constexpr bool is_array_convertible_v = is_array_convertible<T, U>::value;

template <typename T>
struct type_identity;

template <typename T>
using type_identity_t = typename type_identity<T>::type;

template <typename T>
struct is_scoped_enum;

template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

template <typename T>
struct remove_cvref;

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template<typename... T>
struct common_reference;

template<typename... T>
using common_reference_t = typename common_reference<T...>::type;

template<typename T, typename U, template<typename> typename TQual, template<typename> typename UQual>
struct basic_common_reference{};
```

### yat::is_variant_alternative

* `yat::is_variant_alternative` determines if a given type `T` is an alternative type of a `VariantType`
* `yat::is_variant_alternative_t` gives the type of a given `yat::is_variant_alternative` result
* `yat::is_variant_alternative_v` gives the boolean value of a given `yat::is_variant_alternative` result

### yat::is_one_of

* `yat::is_one_of` determines if a given type `T` is the same as any one of the given set of `Types`
* `yat::is_one_of_t` gives the type of a given `yat::is_one_of` result
* `yat::is_one_of_v` gives the boolean value of a given `yat::is_one_of` result

### yat::is_char_type

* `yat::is_char_type` determines if a given type `T` is a character type
* `yat::is_char_type_t` gives the type of a given `yat::is_char_type` result
* `yat::is_char_type_v` gives the boolean value of a given `yat::is_char_type` result

### yat::is_dereferencable

* `yat::is_dereferencable` determines if a given type `T` is dereferencable
* `yat::is_dereferencable_t` gives the type of a given `yat::is_dereferencable` result
* `yat::is_dereferencable_v` gives the boolean value of a given `yat::is_dereferencable` result

### yat::is_array_convertible

* `yat::is_array_convertible` determines if a given array of type `T` is a convertible to an array of type `U`
* `yat::is_array_convertible_t` gives the type of a given `yat::is_array_convertible` result
* `yat::is_array_convertible_v` gives the boolean value of a given `yat::is_array_convertible` result

### yat::type_identity

`yat::type_identity` provides an implementation of [std::type_identity](https://en.cppreference.com/w/cpp/types/type_identity).  

* `yat::type_identity` Provides the member typedef type that names `T` (i.e., the identity transformation).
* `yat::type_identity_t` gives the type of a given `yat::type_identity` result

### yat::is_scoped_enum

* `yat::is_scoped_enum` provides an implementation of [std::is_scoped_enum](https://en.cppreference.com/w/cpp/types/is_scoped_enum).  
* `yat::is_scoped_enum_v` gives the boolean value of a given `yat::is_scoped_enum` result

### yat::remove_cvref

* `yat::remove_cvref` provides an implementation of [std::remove_cvref](https://en.cppreference.com/w/cpp/types/remove_cvref).  
* `yat::remove_cvref_t` gives the type of a given `yat::remove_cvref` result

### yat::common_reference

* `yat::common_reference` provides an implementation of [std::common_reference](https://en.cppreference.com/w/cpp/types/common_reference).  
* `yat::common_reference_t` gives the type of a given `yat::common_reference` result
* `yat::basic_common_reference` is a customization point that allows users to influence the result of common_reference for user-defined types (typically proxy references). The primary template is empty.

## utility.hpp

```cpp
template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T value) noexcept;
```

### yat::to_underlying

`yat::to_underlying` provides an implementation of the proposed `std::to_underlying` from [P1682R0](https://wg21.link/P1682R0).  

It converts an enumeration to its underlying type.

## variant.hpp

```cpp
template <typename... Ts>
struct overloaded;

template <typename Variant, typename... Handlers>
auto match(Variant&& v, Handlers&&... handlers);
```

### yat::overloaded

`yat::overloaded` allows the overloading of lamdas and other function objects.  This is useful for matching on variants and lambdas

### yat::match

`yat::match` is a helper function to provide variant matching on function-like objects

### Apple macOS Support

Apple disallows the use of std::variant before macOS 10.14 because the `std::bad_variant_access` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<variant>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::variant` on those systems.

All of the `std` types in the `<variant>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<variant>` header elsewhere, this may fail to work properly.

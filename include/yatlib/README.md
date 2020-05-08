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

### Macros

* `YAT_HAS_CONSTEXPR_BIT_CAST` is defined when `yat::bit_cast` is `constexpr`
* `YAT_HAS_CONSTEXPR_BYTESWAP` is defined when `yat::byteswap` is `constexpr`

## endian.hpp

```cpp
constexpr bool is_little_endian_system;
constexpr bool is_big_endian_system;

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

## iterator.hpp

```cpp
template <typename T, typename = std::enable_if_t<is_dereferencable_v<T> > >
using iter_reference_t = decltype(*std::declval<T&>());
```

### yat::iter_reference_t

`yat::iter_reference_t` provides an implementation of [std::iter_reference_t](https://en.cppreference.com/w/cpp/iterator/iter_t).  

It computes the reference type of T..

## memory.hpp

```cpp
template <typename Ptr>
constexpr auto to_address(const Ptr& p) noexcept;

template <typename T>
constexpr T* to_address(T* p) noexcept;
```

### yat::to_address

`yat::to_address` provides an implementation of [std::to_address](https://en.cppreference.com/w/cpp/memory/to_address).  

It obtains the address represented by p without forming a reference to the pointee.

## optional.hpp

Apple disallows the use of std::optional before macOS 10.14 because the `std::bad_optional_access` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<optional>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::optional` on those systems.

All of the `std` types in the `<optional>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<optional>` header elsewhere, this may fail to work properly.

## type_traits.hpp

```cpp
template <typename T, typename VariantType>
struct is_variant_member;

template <typename T, typename VariantType>
using is_variant_member_t = typename is_variant_member<T, VariantType>::type;

template <typename T, typename VariantType>
constexpr bool is_variant_member_v = is_variant_member<T, VariantType>::value;

template <typename T, typename... Types>
struct is_one_of;

template <typename T, typename... Types>
using is_one_of_t = typename is_one_of<T, Types...>::type;

template <typename T, typename... Types>
constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

template <typename T>
struct is_char_type;

template <typename T>
constexpr bool is_char_type_v = is_char_type<T>::value;

template <typename T>
using is_char_type_t = typename is_char_type<T>::type;

template <typename T>
struct is_dereferencable;

template <typename T>
using is_dereferencable_t = typename is_dereferencable<T>::type;

template <typename T>
constexpr bool is_dereferencable_v = is_dereferencable<T>::value;

template <typename T>
struct type_identity;

template <typename T>
using type_identity_t = typename type_identity<T>::type;
```

### yat::is_variant_member

* `yat::is_variant_member` determines if a given type `T` is a member type of a `VariantType`
* `yat::is_variant_member_t` gives the type of a given `yat::is_variant_member` result
* `yat::is_variant_member_v` gives the boolean value of a given `yat::is_variant_member` result

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

## utility.hpp

```cpp
template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T value) noexcept;
```

### yat::to_underlying

`yat::to_underlying` provides an implementation of the proposed `std::to_underlying` from [P1682R0](https://wg21.link/P1682R0).  

It converts an enumeration to its underlying type.

### yat::type_identity

`yat::type_identity` provides an implementation of [std::type_identity](https://en.cppreference.com/w/cpp/types/type_identity).  

* `yat::type_identity` Provides the member typedef type that names `T` (i.e., the identity transformation).
* `yat::type_identity_t` gives the type of a given `yat::itype_identity` result

## variant.hpp

Apple disallows the use of std::variant before macOS 10.14 because the `std::bad_variant_access` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<variant>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::variant` on those systems.

All of the `std` types in the `<variant>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<variant>` header elsewhere, this may fail to work properly.

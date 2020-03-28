# YATLib - Features

This document describes the features available in YATLib.  It is organized by header.  When `std` versions of implementations are available, implementations in the `yat` namespace are simply aliases to their `std` counterpart.

## any.hpp

Apple disallows the use of std::any before macOS 10.14 because the `std::bad_any_cast` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<any>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::any` on those systems.

All of the `std` types in the `<any>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<any>` header elsewhere, this may fail to work properly.

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
```

### yat::is_variant_member

* `yat::is_variant_member` determines if a given type `T` is a member type of a `VariantType`
* `yat::is_variant_member_t` is a helper type that is set to either `std::true_type` or `std::false_type` for a given type/variant combination
* `yat::is_variant_member_v` is a convenience type that provides a boolean value for a given type/variant combination

## utility.hpp

```cpp
template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T value) noexcept;
```

### yat::to_underlying

`yat::to_underlying` provides an implementation of the proposed `std::to_underlying` from [P1682R0](https://wg21.link/P1682R0).  

It converts an enumeration to its underlying type.

## variant.hpp

Apple disallows the use of std::variant before macOS 10.14 because the `std::bad_variant_access` implementation is compiled into `libc++.so` and is not available on those systems.  Importing this header instead of `<variant>` provide an inline implementation and attempts to disable the macros that prevent the use of `std::variant` on those systems.

All of the `std` types in the `<variant>` header are aliases into the `yat` namespace.  This helps ensure that you've included the right header.

NOTE: If your project or its dependencies import the `<variant>` header elsewhere, this may fail to work properly.

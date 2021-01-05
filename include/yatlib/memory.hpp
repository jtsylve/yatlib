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
#pragma once

#include <memory>
#include <type_traits>

#include "features.hpp"

/////////////////////////////////////////
// P0653R2 - https://wg21.link/P0653R2 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_to_address) && __cpp_lib_to_address >= 201711L
  #define YAT_INTERNAL_USE_STD_TO_ADDRESS
#endif

#ifdef YAT_INTERNAL_USE_STD_TO_ADDRESS

namespace yat {
using std::to_address;
}  // namespace yat

#else

  #ifdef YAT_IS_MSVC
// MSVC's standard library has a std::_Get_first_parameter helper type that
// doesn't have a specialization for types with type template parameters.  This
// breaks std::pointer_traits.  This specialization is a workaround.
template <template <typename, size_t> typename T, typename U, std::size_t N>
struct std::_Get_first_parameter<T<U, N>> {
  using type = U;
};
  #endif

namespace yat::detail {

template <typename, typename = void>
constexpr bool has_pt_to_address_v = false;

template <typename T>
constexpr bool has_pt_to_address_v<
    T, std::void_t<decltype(std::pointer_traits<T>::to_address)>> = true;

}  // namespace yat::detail

namespace yat {

/// Obtain the address represented by p without forming a reference to the
/// pointee.
template <typename T>
constexpr T* to_address(T* p) noexcept {
  static_assert(!std::is_function_v<T>);
  return p;
}

/// Obtain the address represented by p without forming a reference to the
/// pointee.
template <typename T>
constexpr auto to_address(const T& p) noexcept {
  if constexpr (detail::has_pt_to_address_v<T>) {
    return std::pointer_traits<T>::to_address(p);
  } else {
    return to_address(p.operator->());
  }
}

#endif  // YAT_INTERNAL_USE_STD_TO_ADDRESS

/// A light-weight reference counted smart pointer that is similar to
/// std::shared_ptr, but is not thread-safe and lacks many of the bells and
/// whistles.
template <typename T>
class refcnt_ptr {
  static_assert(!std::is_same_v<std::decay_t<T>, void>,
                "refcnt_ptr does not currently support type-erased pointers");

  static_assert(!std::is_array_v<T>,
                "refcnt_ptr does not currently support array allocations");

 public:
  using element_type = std::remove_extent_t<T>;

  /// Constructs a refcnt_ptr with no managed object, i.e. empty refcnt_ptr
  constexpr refcnt_ptr() noexcept = default;

  // cppcheck-suppress noExplicitConstructor
  /// Constructs a refcnt_ptr with no managed object, i.e. empty refcnt_ptr
  constexpr refcnt_ptr(std::nullptr_t) noexcept : refcnt_ptr() {}

  /// Constructs a refcnt_ptr with ptr as the pointer to the managed object.
  ///
  /// Y* must be convertible to T*.
  template <typename Y,
            typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
  explicit refcnt_ptr(Y* ptr) : _value{ptr}, _refcnt{new uint_fast32_t(1)} {}

  /// Constructs a refcnt_ptr which shares ownership of the object managed by
  /// other.
  ///
  /// If other manages no object, *this manages no object too.
  refcnt_ptr(const refcnt_ptr& other) noexcept
      : _value{other._value}, _refcnt{other._refcnt} {
    if (_refcnt != nullptr) {
      ++(*_refcnt);
    }
  }

  /// Shares ownership of the object managed by rhs. If rhs manages no object,
  /// *this manages no object too.
  refcnt_ptr& operator=(const refcnt_ptr& rhs) noexcept {
    if (&rhs != this) {
      *this = refcnt_ptr(rhs);
    }

    return (*this);
  }

  /// Constructs a refcnt_ptr which shares ownership of the object managed by
  /// other.
  ///
  /// If other manages no object, *this manages no object too.
  ///
  /// The template overload doesn't participate in overload resolution if Y* is
  /// not convertible to T*
  template <typename Y,
            typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
  refcnt_ptr(const refcnt_ptr<Y>& other) noexcept
      : _value{other._value}, _refcnt{other._refcnt} {
    if (_refcnt != nullptr) {
      ++(*_refcnt);
    }
  }

  /// Move-constructs a refcnt_ptr from other.
  ///
  /// After the construction, *this contains a copy of the previous state of
  /// other, other is empty and its stored pointer is null.
  refcnt_ptr(refcnt_ptr&& other) noexcept {
    std::swap(_value, other._value);
    std::swap(_refcnt, other._refcnt);
  }

  /// Move-assigns a shared_ptr from rhs. After the assignment, *this contains a
  /// copy of the previous state of rhs, and rhs is empty.
  refcnt_ptr& operator=(refcnt_ptr&& rhs) noexcept {
    if (&rhs != this) {
      this->~refcnt_ptr();
      new (this) refcnt_ptr(std::forward<refcnt_ptr>(rhs));
    }

    return (*this);
  }

  /// Move-constructs a shared_ptr from other.
  ///
  /// After the construction, *this contains a copy of the previous state of
  /// other, other is empty and its stored pointer is null.
  ///
  /// The template overload doesn't participate in overload resolution if Y* is
  /// not convertible to T*.
  template <typename Y,
            typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
  refcnt_ptr(refcnt_ptr<Y>&& other) noexcept {
    std::swap(_value, other._value);
    std::swap(_refcnt, other._refcnt);
  }

  /// The aliasing constructor: constructs a refcnt_ptr which shares ownership
  /// information with the initial value of r, but holds an unrelated and
  /// unmanaged pointer ptr.
  template <typename Y>
  refcnt_ptr(const refcnt_ptr<Y>& r, T* ptr) noexcept
      : _value{ptr}, _refcnt{(ptr) ? r._refcnt : nullptr} {
    if (_refcnt != nullptr) {
      ++(*_refcnt);
    }
  }

  /// If *this owns an object and it is the last refcnt_ptr owning it, the
  /// object is destroyed through the owned deleter.
  ///
  /// After the destruction, the smart pointers that shared ownership with
  /// *this, if any, will report a use_count() that is one less than its
  /// previous value.
  YAT_ALWAYS_INLINE ~refcnt_ptr() {
    if (_refcnt != nullptr && --(*_refcnt) == 0) {
      delete _refcnt;

      if constexpr (std::is_destructible_v<T>) {
        if (_value != nullptr) {
          delete _value;
        }
      }
    }
  }

  /// Releases the ownership of the managed object, if any.
  ///
  /// After the call, *this manages no object
  void reset() noexcept {
    this->~refcnt_ptr();
    new (this) refcnt_ptr();
  }

  /// Replaces the managed object with an object pointed to by ptr.
  ///
  /// Y must be a complete type and convertible to T.
  template <typename Y,
            typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
  void reset(Y* ptr) {
    this->~refcnt_ptr();
    new (this) refcnt_ptr(ptr);
  }

  /// Exchanges the stored pointer values and the ownerships of *this and other.
  ///
  /// Reference counts, if any, are not adjusted.
  void swap(refcnt_ptr& other) noexcept {
    std::swap(_value, other._value);
    std::swap(_refcnt, other._refcnt);
  }

  /// Returns the stored pointer.
  element_type* get() const noexcept { return _value; }

  /// The result of dereferencing the stored pointer, i.e., *get()
  auto& operator*() const noexcept {
    if constexpr (!std::is_same_v<T, void>) {
      return *_value;
    }
  }

  /// The stored pointer, i.e., get()
  element_type* operator->() const noexcept { return _value; }

  /// Returns the number of different refcnt_ptr instances (this included)
  /// managing the current object.
  ///
  /// If there is no managed object, ​0​ is returned
  uint_fast32_t use_count() const noexcept {
    if (_refcnt != nullptr) {
      return *_refcnt;
    }

    return 0;
  }

  /// Checks if *this is the only shared_ptr instance managing the current
  /// object, i.e. whether use_count() == 1
  bool unique() const noexcept { return use_count() == 1; }

  /// Checks if *this stores a non-null pointer, i.e. whether get() != nullptr
  explicit operator bool() const noexcept { return _value != nullptr; }

 private:
  refcnt_ptr(T* value, uint_fast32_t* refcnt) noexcept
      : _value{value}, _refcnt{refcnt} {}

  T* _value{};
  uint_fast32_t* _refcnt{};

  template <typename>
  friend class refcnt_ptr;

  //
  // Comparison operators
  //

  template <typename U>
  friend bool operator==(const refcnt_ptr& lhs,
                         const refcnt_ptr<U>& rhs) noexcept {
    return lhs._refcnt == rhs._refcnt;
  }

  template <typename U>
  friend bool operator!=(const refcnt_ptr& lhs,
                         const refcnt_ptr<U>& rhs) noexcept {
    return lhs._refcnt != rhs._refcnt;
  }

  template <typename U>
  friend bool operator<(const refcnt_ptr& lhs,
                        const refcnt_ptr<U>& rhs) noexcept {
    return lhs._refcnt < rhs._refcnt;
  }

  template <typename U>
  friend bool operator>(const refcnt_ptr& lhs,
                        const refcnt_ptr<U>& rhs) noexcept {
    return lhs._refcnt > rhs._refcnt;
  }

  template <typename U>
  friend bool operator<=(const refcnt_ptr& lhs,
                         const refcnt_ptr<U>& rhs) noexcept {
    return lhs._refcnt <= rhs._refcnt;
  }

  template <typename U>
  friend bool operator>=(const refcnt_ptr& lhs,
                         const refcnt_ptr<U>& rhs) noexcept {
    return lhs._refcnt >= rhs._refcnt;
  }
};

/// Constructs an object of type T and wraps it in a yat::refcnt_ptr using
/// args
/// as the parameter list for the constructor of T.
template <typename T, typename... Args>
inline refcnt_ptr<T> make_refcnt(Args&&... args) {
  return refcnt_ptr<T>{new T(std::forward<Args>(args)...)};
}

/// Creates a new instance of yat::refcnt_ptr whose stored pointer is obtained
/// from r's stored pointer using a cast expression.
///
/// If r is empty, so is the new refcnt_ptr (but its stored pointer is not
/// necessarily null). Otherwise, the new refcnt_ptr will share ownership with
/// the initial value of r.
template <typename T, typename U>
inline refcnt_ptr<T> static_pointer_cast(const refcnt_ptr<U>& r) noexcept {
  return {r, static_cast<T*>(r.get())};
}

/// Creates a new instance of yat::refcnt_ptr whose stored pointer is obtained
/// from r's stored pointer using a cast expression.
///
/// If r is empty, so is the new refcnt_ptr (but its stored pointer is not
/// necessarily null). Otherwise, the new refcnt_ptr will share ownership with
/// the initial value of r, except that it is empty if the dynamic_cast
/// performed by dynamic_pointer_cast returns a null pointer.
template <typename T, typename U>
inline refcnt_ptr<T> dynamic_pointer_cast(const refcnt_ptr<U>& r) noexcept {
  return {r, dynamic_cast<T*>(r.get())};
}

/// Creates a new instance of yat::refcnt_ptr whose stored pointer is obtained
/// from r's stored pointer using a cast expression.
///
/// If r is empty, so is the new refcnt_ptr (but its stored pointer is not
/// necessarily null). Otherwise, the new refcnt_ptr will share ownership with
/// the initial value of r, except that it is empty if the dynamic_cast
/// performed by dynamic_pointer_cast returns a null pointer.
template <typename T, typename U>
inline refcnt_ptr<T> const_pointer_cast(const refcnt_ptr<U>& r) noexcept {
  return {r, const_cast<T*>(r.get())};
}

/// Creates a new instance of yat::refcnt_ptr whose stored pointer is obtained
/// from r's stored pointer using a cast expression.
///
/// If r is empty, so is the new refcnt_ptr (but its stored pointer is not
/// necessarily null). Otherwise, the new refcnt_ptr will share ownership with
/// the initial value of r, except that it is empty if the dynamic_cast
/// performed by dynamic_pointer_cast returns a null pointer.
template <typename T, typename U>
inline refcnt_ptr<T> reinterpret_pointer_cast(const refcnt_ptr<U>& r) noexcept {
  return {r, reinterpret_cast<T*>(r.get())};
}

}  // namespace yat

//
// Overloads
//
namespace std {

/// Specializes the std::swap algorithm for yat::refcnt_ptr.
///
/// Swaps the contents of lhs and rhs by calling lhs.swap(rhs).
template <typename T>
void swap(yat::refcnt_ptr<T>& lhs, yat::refcnt_ptr<T>& rhs) noexcept {
  return lhs.swap(rhs);
}

/// The template specialization of std::hash for yat::refcnt_ptr<T> allows users
/// to obtain hashes of objects of type yat::refcnt_ptr<T>.
template <typename T>
class hash<yat::refcnt_ptr<T>> {
 public:
  size_t operator()(const yat::refcnt_ptr<T>& ptr) const {
    return hash<decltype(ptr.get())>(ptr.get());
  }
};

}  // namespace std

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_TO_ADDRESS

#pragma once

#include <string_view>

#include "features.hpp"

/////////////////////////////////////////
// P1402R0 - https://wg21.link/P1402R0 //
////////////////////////////////////////

namespace yat {

/// A tag that is used to specify that a string view with size is null
/// terminated
struct null_terminated_t {};

/// A special type of std::string_view that guarentees that strings are null
/// terminated
template <typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_cstring_view : public std::basic_string_view<CharT, Traits> {
  using string_view_type = std::basic_string_view<CharT, Traits>;
  using typename string_view_type::const_pointer;
  using typename string_view_type::size_type;

  using string_view_type::basic_string_view;

  // Removed functionality
  constexpr basic_cstring_view(const CharT*, size_type) = delete;
  constexpr void remove_suffix(size_type n) = delete;

  /// Constructs a view of the first count characters of the character array
  /// starting with the element pointed by s. s can contain null characters. The
  /// behavior is undefined if [s, s+count) is not a valid range (even though
  /// the constructor may not access any of the elements of this range). After
  /// construction, data() is equal to s, and size() is equal to count.
  ///
  /// Caller must ensure that str[len] == 0.
  constexpr basic_cstring_view(null_terminated_t, const CharT* str,
                               size_type len)
      : string_view_type(str, len) {}

  /// Creates a view based on a standard string_view.
  ///
  /// Caller must ensure that the character array is null-terminated
  constexpr basic_cstring_view(null_terminated_t,
                               const string_view_type& sv) noexcept
      : string_view_type(sv) {}

  /// Creates a null-terminated subview starting at position pos
  [[nodiscard]] constexpr basic_cstring_view substr(size_type pos) const {
    return {null_terminated_t{}, this->data() + pos, this->size() - pos};
  }

  /// Returns a view of the substring [pos, pos + rcount), where rcount is the
  /// smaller of count and size() - pos.
  [[nodiscard]] constexpr string_view_type substr(size_type pos,
                                                  size_type count) const {
    return string_view_type::substr(pos, count);
  }

  /// Returns access to the null-terminated string.
  [[nodiscard]] constexpr const_pointer c_str() const noexcept {
    return this->data();
  }
};

using cstring_view = basic_cstring_view<char>;
using u16cstring_view = basic_cstring_view<char16_t>;
using u32cstring_view = basic_cstring_view<char32_t>;
using wcstring_view = basic_cstring_view<wchar_t>;

#ifdef YAT_SUPPORTS_CPP20
using u8cstring_view = basic_cstring_view<char8_t>;
#endif

}  // namespace yat

namespace std {

template <typename CharT>
struct hash<yat::basic_cstring_view<CharT, char_traits<CharT>>>
    : public hash<basic_string_view<CharT, char_traits<CharT>>> {};

}  // namespace std
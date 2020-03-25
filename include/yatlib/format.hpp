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

#if __has_include("format")
  #include <format>
#endif

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

///////////////////////////////////////////
// P0645R10 - https://wg21.link/P0645R10 //
///////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_format) && __cpp_lib_endian >= 201907L
  #define YAT_INTERNAL_USE_STD_FORMAT
#endif

#ifdef YAT_INTERNAL_USE_STD_FORMAT

namespace yat {

// classes
using std::basic_format_parse_context;
using std::format_parse_context;
using std::wformat_parse_context;
// using std::formatter;
// using std::basic_format_context;
// using std::basic_format_arg;
// using std::basic_format_args;
// using std::format_args;
// using std::format_args_t;
// using std::format_context;
using std::format_error;
// using std::format_to_n_result;
// using std::wformat_args;
// using std::wformat_context;

// functions
// using std::format;
// using std::format_to;
// using std::format_to_n;
// using std::formatted_size;
// using std::make_format_args;
// using std::make_wformat_args;
// using std::vformat;
// using std::vformat_to;
// using std::visit_format_arg;

}  // namespace yat

#else

namespace yat {

// [format.error]

/// The class format_error defines the type of objects thrown as exceptions to
/// report errors from the formatting library.
class format_error : public std::runtime_error {
 public:
  using runtime_error::runtime_error;
};

// [format.parse_context]

/// Provides access to the format string parsing state consisting of the format
/// string range being parsed and the argument counter for automatic indexing.
///
/// A basic_format_parse_context instance is passed to Formatter when parsing
/// the format specification.
template <typename CharT>
class basic_format_parse_context {
 public:
  // Member types
  using char_type = CharT;
  using const_iterator = typename std::basic_string_view<CharT>::const_iterator;
  using iterator = const_iterator;

  /// Constructs a std::basic_format_parse_context instance from format string
  /// and argument count.
  constexpr explicit basic_format_parse_context(
      std::basic_string_view<CharT> fmt,
      [[maybe_unused]] std::size_t num_args = 0) noexcept
      : _begin{fmt.cbegin()}, _end{fmt.cend()} {}

  // not copyable
  basic_format_parse_context(const basic_format_parse_context&) = delete;
  basic_format_parse_context& operator=(const basic_format_parse_context&) =
      delete;

  /// Returns an iterator to the beginning of the format string range.
  [[nodiscard]] constexpr const_iterator begin() const noexcept {
    return _begin;
  }

  /// Returns an iterator to the end of the format string range.
  [[nodiscard]] constexpr const_iterator end() const noexcept { return _end; }

  /// Sets the beginning of the format string range to it. After a call to
  /// advance_to, subsequent calls to begin() will return a copy of it.
  ///
  /// The behavior is undefined if end() is not reachable from it.
  constexpr void advance_to(const_iterator it) { _begin = it; }

  /// Enters automatic argument indexing mode, and returns the next argument
  /// index, starting from 0.
  ///
  /// If *this has already entered manual argument indexing mode, throws
  /// std::format_error.
  constexpr std::size_t next_arg_id() {
    if (_next_arg_id == manual_mode_arg_id) {
      throw format_error(
          "cannot switch from manual to automatic argument indexing");
    }

    return _next_arg_id++;
  }

  /// Enters manual argument indexing mode.
  ///
  /// If *this has already entered automatic argument indexing mode, throws
  /// std::format_error.
  ///
  /// If id is larger than or equal to the argument count provided in the
  /// constructor, the call is not a constant expression.
  constexpr void check_arg_id([[maybe_unused]] std::size_t id) {
    if (_next_arg_id == std::size_t{}) {
      _next_arg_id = manual_mode_arg_id;
    } else if (_next_arg_id != manual_mode_arg_id) {
      throw format_error(
          "cannot switch from automatic to manual argument indexing");
    }
  }

 private:
  static constexpr std::size_t manual_mode_arg_id =
      std::numeric_limits<std::size_t>::max();

  std::size_t _next_arg_id{};

  const_iterator _begin;
  const_iterator _end;
};

using format_parse_context = basic_format_parse_context<char>;
using wformat_parse_context = basic_format_parse_context<wchar_t>;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_FORMAT

// Clean up internal macros
#undef YAT_INTERNAL_USE_STD_FORMAT

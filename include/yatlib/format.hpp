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

#include "type_traits.hpp"

#if __has_include("format")
  #include <format>
#endif

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>

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
using std::basic_format_arg;
using std::basic_format_args;
using std::basic_format_context;
using std::basic_format_parse_context;
using std::format_parse_context;
using std::formatter;
using std::wformat_parse_context;
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
using std::make_format_args;
// using std::make_wformat_args;
// using std::vformat;
// using std::vformat_to;
using std::visit_format_arg;

}  // namespace yat

#else

// Forward declarations
namespace yat::detail {

template <typename Context>
class basic_format_args_store;

}  // namespace yat::detail

// Forward declarations
namespace yat {

class format_error;

template <typename T, typename CharT>
struct formatter;

template <typename CharT>
class basic_format_parse_context;

using format_parse_context = basic_format_parse_context<char>;
using wformat_parse_context = basic_format_parse_context<wchar_t>;

template <typename Context>
class basic_format_arg;

template <typename Visitor, typename Context>
constexpr auto visit_format_arg(Visitor&& vis, basic_format_arg<Context> arg)
    -> decltype(vis(0));

template <typename Context, typename... Args>
detail::basic_format_args_store<Context> make_format_args(const Args&... args);

template <typename Context>
class basic_format_args;

template <typename OutputIt, typename CharT>
class basic_format_context;

}  // namespace yat

namespace yat {

/// The class format_error defines the type of objects thrown as exceptions to
/// report errors from the formatting library.
class format_error : public std::runtime_error {
 public:
  using runtime_error::runtime_error;
};

/// The enabled specializations of formatter define formatting rules for a given
/// type. Enabled specializations meet the Formatter requirements.
template <typename T, typename CharT = char>
struct formatter {
  // Disabled formatters are not default constructible
  formatter() = delete;

  // Disabled formatters are not copyable
  formatter(const formatter&) = delete;
  formatter& operator=(const formatter&) = delete;

  // Disabled formatters are not moveable
  formatter(formatter&&) = delete;
  formatter& operator=(formatter&&) = delete;
};

// [format.parse_context]

/// Provides access to the format string parsing state consisting of the
/// format string range being parsed and the argument counter for automatic
/// indexing.
///
/// A basic_format_parse_context instance is passed to Formatter when
/// parsing the format specification.
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

template <typename Context>
class basic_format_arg {
  using char_type = typename Context::char_type;
  using format_parse_type = basic_format_parse_context<char_type>;

 public:
  constexpr basic_format_arg() noexcept = default;

  constexpr explicit operator bool() const noexcept {
    return !std::holds_alternative<std::monostate>(_value);
  }

  class handle {
    std::function<void(format_parse_type&, Context&)> _format;

    template <typename T>
    using formatter_type = typename Context::template formatter_type<T>;

    template <typename T>
    explicit handle(const T& val) noexcept {
      _format = [val](format_parse_type& parse_ctx, Context& format_ctx) {
        formatter_type<T> f{};
        parse_ctx.advance_to(f.parse(parse_ctx));
        format_ctx.advance_to(f.format(val, format_ctx));
      };
    }

   public:
    void format(format_parse_type& parse_ctx, Context& format_ctx) {
      std::invoke(_format, parse_ctx, format_ctx);
    }

    friend class basic_format_arg;
  };

 private:
  using storage_type =
      std::variant<std::monostate, bool, char_type, int, unsigned int,
                   long long int, unsigned long long int, float, double,
                   long double, const char_type*,
                   std::basic_string_view<char_type>, const void*, handle>;

  template <typename T, typename = std::enable_if_t<is_variant_member_v<
                            std::decay_t<T>, storage_type> > >
  constexpr explicit basic_format_arg(T&& value) noexcept
      : _value(std::forward<T>(value)) {}

  storage_type _value{};

  /// Applies the visitor vis to the object contained in arg.
  template <typename Visitor, typename Ctx>
  friend constexpr auto visit_format_arg(Visitor&& vis,
                                         basic_format_arg<Ctx> arg)
      -> decltype(vis(0)) {
    return std::visit(std::forward<Visitor>(vis), arg._value);
  }

  friend class detail::basic_format_args_store<Context>;
};

template <typename Context, typename... Args>
detail::basic_format_args_store<Context> make_format_args(const Args&... args) {
  return detail::basic_format_args_store<Context>{args...};
}

template <typename Context>
class basic_format_args {
 public:
  /// Constructs a yat::basic_format_args object that does not hold any
  /// formatting argument.
  constexpr basic_format_args() noexcept = default;

  // cppcheck-suppress noExplicitConstructor
  /// Constructs a basic_format_args object from the result of a call to
  /// yat::make_format_args or yat::make_wformat_args.
  ///
  /// yat::basic_format_args has reference semantics. It is the programmer's
  /// responsibility to ensure that *this does not outlive store (which, in
  /// turn, should not outlive the arguments to yat::make_format_args or
  /// yat::make_wformat_args).
  constexpr basic_format_args(
      const detail::basic_format_args_store<Context>& store) noexcept
      : _store{&store} {}

  /// Returns a yat::basic_format_arg holding the i-th argument in args, where
  /// args is the parameter pack passed to yat::make_format_args or
  /// yat::make_wformat_args.
  ///
  /// If there's no such formatting argument (i.e. *this was default-constructed
  /// or i is not less than the number of formatting arguments), returns a
  /// default-constructed yat::basic_format_arg (holding a std::monostate
  /// object).
  basic_format_arg<Context> get(std::size_t i) const noexcept {
    if (_store == nullptr || _store->size() <= i) {
      return {};
    }

    return (*_store)[i];
  }

 private:
  const detail::basic_format_args_store<Context>* _store{};  // unowned
};

/// Provides access to formatting state consisting of the formatting arguments
/// and the output iterator.
///
/// The behavior is undefined if OutputIt does not model
/// std::output_iterator<const CharT&>.
template <typename OutputIt, typename CharT>
class basic_format_context {
 public:
  using iterator = OutputIt;
  using char_type = CharT;

  template <typename T>
  using formatter_type = formatter<T, CharT>;

  /// Returns the iterator to the output buffer.
  iterator out() { return _out; }

  /// Sets the output iterator to it. After a call to advance_to, subsequent
  /// calls to out() will return a copy of it.
  void advance_to(iterator it) { _out = it; }

  /// Returns a yat::basic_format_arg holding the id-th argument in args, where
  /// args is the parameter pack or yat::basic_format_args object passed to the
  /// formatting function.
  ///
  /// If id is not less than the number of formatting arguments, returns a
  /// default-constructed yat::basic_format_arg (holding a std::monostate
  /// object).
  basic_format_arg<basic_format_context> arg(size_t id) const {
    return _args.get(id);
  }

 private:
  iterator _out{};
  basic_format_args<basic_format_context> _args{};
};

}  // namespace yat

namespace yat::detail {

template <typename Context>
class basic_format_args_store : public std::vector<basic_format_arg<Context> > {
 public:
  template <typename... Args>
  explicit basic_format_args_store(Args&&... args)
      : std::vector<basic_format_arg<Context> >(
            {basic_format_arg<Context>{std::forward<Args>(args)}...}) {}
};

}  // namespace yat::detail

#endif  // YAT_INTERNAL_USE_STD_FORMAT

// Clean up internal macros
#undef YAT_INTERNAL_USE_STD_FORMAT

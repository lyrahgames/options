#pragma once
#include <lyrahgames/options/basic_option.hpp>
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

/// Option type which provides an additional value
/// by using '=' without white space as an assignment.
/// It must not provide a short name alternative due to the assignment pattern.
/// Hence, no option identifier is used.
/// Usage: --key=value
template <static_zstring N, static_zstring D>
struct assignment : basic_option<czstring, N, D> {
  using base = basic_option<czstring, N, D>;

  using base::name;
  using base::value;

  static constexpr auto help() noexcept {
    return static_zstring("--") + name() + static_zstring("=<value>");
  }

  constexpr operator bool() const noexcept { return value(); }

  constexpr bool parse(czstring call, arg_list& args) {
    const auto tmp = name();  // Need this one due to optimization.
    czstring n = tmp;
    while (*n && *call && (*n == *call)) ++n, ++call;
    if (!*n && !*call) {
      args.unpop_front();
      throw parser_error(
          args, string("No assignment for option '") + args.pop_front() + "'.");
    }
    if (*n) return false;
    if (*call != '=') return false;
    value() = call + 1;
    return true;
  }

  constexpr void tree_parse(czstring current, arg_list& args) {
    if (*current++ != '=') {
      args.unpop_front();
      throw parser_error(
          args, string("Failed to parse option '") + args.front() + "'.");
    }
    value() = current;
  }
};

}  // namespace lyrahgames::options

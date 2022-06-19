#pragma once
#include <lyrahgames/options/option_utility.hpp>

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

  constexpr bool parse(czstring current, arg_list& args) {
    if (*current == '=') {
      value() = ++current;
      return true;
    }
    // Error handling
    if (!*current) {
      args.unpop_front();
      throw parser_error(
          args, string("No assignment for option '") + args.front() + "'.");
    }
    return false;
  }
};

}  // namespace lyrahgames::options

#pragma once
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

/// Option type for list of options which also provides one additional value.
/// Usage: --key=value

template <static_zstring N, static_zstring D>
struct assignment {
  using value_type = czstring;

  static constexpr auto name() { return N; }

  static constexpr auto description() { return D; }

  static constexpr auto help() {
    return static_zstring("--") + N + static_zstring("=VALUE");
  }

  constexpr operator czstring() { return val; }

  constexpr auto value() noexcept -> value_type& { return val; }

  constexpr auto value() const noexcept -> value_type { return val; }

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
    val = call + 1;
    return true;
  }

  czstring val = nullptr;
};

}  // namespace lyrahgames::options

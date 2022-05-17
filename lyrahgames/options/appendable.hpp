#pragma once
#include <vector>
//
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

/// Option type for list of options which also provides one additional value.
/// Usage: --flag value

/// Short Name Alternative
template <static_zstring N, static_zstring D, char S = '\0'>
struct appendable : appendable<N, D, '\0'> {
  using base = appendable<N, D, '\0'>;
  using base::description;
  using base::name;
  using base::parse;
  using base::val;
  using base::value;
  using typename base::value_type;

  static constexpr auto short_name() noexcept { return S; }

  static constexpr auto help() {
    return static_zstring("-") + S + static_zstring(", ") + base::help();
  }

  constexpr bool parse(char c, arg_list& args) {
    if (c != short_name()) return false;
    if (args.empty()) {
      throw parser_error(
          args, string("No given value for short option '") + c + "'.");
    }
    val.push_back(args.pop_front());
    return true;
  }
};

/// Base Implementation
template <static_zstring N, static_zstring D>
struct appendable<N, D, '\0'> {
  using value_type = vector<czstring>;

  static constexpr auto name() { return N; }

  static constexpr auto description() { return D; }

  static constexpr auto help() {
    return static_zstring("--") + N + static_zstring(" <value>");
  }

  constexpr operator bool() noexcept { return !val.empty(); }

  constexpr auto value() noexcept -> value_type& { return val; }

  constexpr auto value() const noexcept -> const value_type& { return val; }

  constexpr bool parse(czstring call, arg_list& args) {
    if (strcmp(call, name())) return false;
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(args, string("No given value for option '") +
                                   args.pop_front() + "'.");
    }
    val.push_back(args.pop_front());
    return true;
  }

  value_type val{};
};

}  // namespace lyrahgames::options

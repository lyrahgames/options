#pragma once
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

/// Option type which is used as a simple switch.
/// If its name is provide as argument its value will be set to 'true'.
/// Otherwise, it will be 'false'.
/// Typical Appearance: --flag, --help

/// Short Name Alternative
template <static_zstring N, static_zstring D, char S = '\0'>
struct flag : flag<N, D, '\0'> {
  using base = flag<N, D, '\0'>;
  using base::description;
  using base::name;
  using base::parse;
  using base::val;
  using base::value;
  using typename base::value_type;

  static constexpr auto short_name() { return S; }

  static constexpr auto help() {
    return static_zstring("-") + S + static_zstring(", ") + base::help();
  }

  constexpr bool parse(char c, arg_list& args) {
    if (c != short_name()) return false;
    return (val = true);
  }
};

/// Implementation that does not provide a short name.
/// It will also be used as base for the short name variant.
/// Name and description will be provided as
/// template parameters by using static C strings.
template <static_zstring N, static_zstring D>
struct flag<N, D, '\0'> {
  using value_type = bool;

  static constexpr auto name() { return N; }

  static constexpr auto description() { return D; }

  static constexpr auto help() { return static_zstring("--") + N; }

  constexpr operator bool() { return val; }

  constexpr auto value() noexcept -> value_type& { return val; }

  constexpr auto value() const noexcept -> value_type { return val; }

  constexpr bool parse(czstring call, arg_list& args) {
    if (std::strcmp(call, name())) return false;
    return (val = true);
  }

  bool val{false};
};

}  // namespace lyrahgames::options

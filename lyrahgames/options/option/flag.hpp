#pragma once
#include <lyrahgames/options/basic_option.hpp>
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

/// Option type which is used as a simple switch.
/// If its name is provide as argument its value will be set to 'true'.
/// Otherwise, it will be 'false'.
/// By using an option identifier, a optional short name can be provided.
/// Typical Appearance: --flag, --help, -h
template <option_identifier N, static_zstring D>
struct flag : basic_option<bool, N, D> {
  using base = basic_option<bool, N, D>;

  using base::has_short_name;
  using base::name;
  using base::short_name;
  using base::value;

  /// The static C string describing the usage of the flag.
  static constexpr auto help() noexcept {
    const auto result = static_zstring("--") + name();
    if constexpr (has_short_name())
      return static_zstring("-") + short_name() + static_zstring(", ") + result;
    else
      return result;
  }

  /// Check if option has been provided at the command line.
  constexpr operator bool() const noexcept { return value(); }

  /// Parser function for the standard flag.
  constexpr bool parse(czstring call, arg_list& args) {
    if (strcmp(call, name())) return false;
    return (value() = true);
  }

  /// Parser function for the short name alternative.
  constexpr bool parse(char c, arg_list& args) requires(has_short_name()) {
    if (c != short_name()) return false;
    return (value() = true);
  }
};

}  // namespace lyrahgames::options

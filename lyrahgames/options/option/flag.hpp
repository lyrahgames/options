#pragma once
#include <lyrahgames/options/basic_option.hpp>
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

template <option_identifier N, static_zstring D>
struct flag : basic_option<bool, N, D> {
  using base = basic_option<bool, N, D>;

  using base::has_short_name;
  using base::name;
  using base::short_name;
  using base::value;

  static constexpr auto help() noexcept {
    const auto result = static_zstring("--") + name();
    if constexpr (has_short_name())
      return static_zstring("-") + short_name() + static_zstring(", ") + result;
    else
      return result;
  }

  constexpr operator bool() noexcept { return value(); }

  constexpr bool parse(czstring call, arg_list& args) {
    if (strcmp(call, name())) return false;
    return (value() = true);
  }

  constexpr bool parse(char c, arg_list& args) requires(has_short_name()) {
    if (c != short_name()) return false;
    return (value() = true);
  }
};

}  // namespace lyrahgames::options

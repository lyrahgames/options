#pragma once
#include <lyrahgames/options/option_utility.hpp>

namespace lyrahgames::options {

/// Option type for list of options which also provides one additional value.
/// Usage: --flag value
template <option_identifier N, static_zstring D>
struct attachment : basic_option<czstring, N, D> {
  using base = basic_option<czstring, N, D>;

  using base::has_short_name;
  using base::name;
  using base::short_name;
  using base::value;

  static constexpr auto help() noexcept {
    const auto result =
        static_zstring("--") + name() + static_zstring(" <value>");
    if constexpr (has_short_name())
      return static_zstring("-") + short_name() + static_zstring(", ") + result;
    else
      return result;
  }

  constexpr operator bool() const noexcept { return value(); }

  constexpr void parse(czstring current, arg_list& args, size_t position) {
    value() = current;
  }

  constexpr void parse(czstring current, arg_list& args) {
    if (*current) {
      args.unpop_front();
      throw parser_error(args, string("Failed to parse option '") +
                                   args.front() + "'. Did you mean '" +
                                   czstring(name()) + "'?");
    }
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(
          args, string("No given value for option '") + args.front() + "'.");
    }
    value() = args.pop_front();
  }

  constexpr void parse(arg_list& args) {
    if (args.empty()) {
      throw parser_error(args, string("No given value for short option '") +
                                   short_name() + "'.");
    }
    value() = args.pop_front();
  }
};

}  // namespace lyrahgames::options

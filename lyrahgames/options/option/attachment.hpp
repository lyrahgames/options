#pragma once
#include <lyrahgames/options/basic_option.hpp>
#include <lyrahgames/options/parser.hpp>

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

  constexpr bool parse(czstring call, arg_list& args) {
    if (strcmp(call, name())) return false;
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(args, string("No given value for option '") +
                                   args.pop_front() + "'.");
    }
    value() = args.pop_front();
    return true;
  }

  constexpr bool parse(char c, arg_list& args) requires(has_short_name()) {
    if (c != short_name()) return false;
    if (args.empty()) {
      throw parser_error(
          args, string("No given value for short option '") + c + "'.");
    }
    value() = args.pop_front();
    return true;
  }

  constexpr void parse(czstring current, arg_list& args, size_t position) {
    value() = current;
  }
};

}  // namespace lyrahgames::options

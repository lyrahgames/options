#pragma once
#include <lyrahgames/options/option_utility.hpp>

namespace lyrahgames::options {

template <option_identifier N, static_zstring D>
struct appendable : basic_option<vector<czstring>, N, D> {
  using base = basic_option<vector<czstring>, N, D>;

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

  constexpr operator bool() const noexcept { return !value().empty(); }

  constexpr void parse(czstring current, arg_list& args, size_t position) {
    value().push_back(current);
  }

  constexpr void parse(czstring current, arg_list& args) {
    if (*current)
      throw parser_error(args, string("Option could not be parsed!"));
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(
          args, string("No given value for option '") + args.front() + "'.");
    }
    value().push_back(args.pop_front());
  }

  constexpr void parse(arg_list& args) requires(has_short_name()) {
    if (args.empty()) {
      throw parser_error(args, string("No given value for short option '") +
                                   short_name() + "'.");
    }
    value().push_back(args.pop_front());
  }
};

}  // namespace lyrahgames::options

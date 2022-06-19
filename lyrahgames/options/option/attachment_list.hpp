#pragma once
#include <lyrahgames/options/option_utility.hpp>

namespace lyrahgames::options {

template <static_zstring N, static_zstring D>
struct attachment_list : basic_option<vector<czstring>, N, D> {
  using base = basic_option<vector<czstring>, N, D>;

  using base::name;
  using base::value;

  static constexpr auto help() noexcept {
    return static_zstring("--") + N + static_zstring(" <value> [<value> ...]");
  }

  constexpr operator bool() const noexcept { return !value().empty(); }

  constexpr bool parse(czstring current, arg_list& args) {
    if (*current) return false;
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(args, string("No given value for option '") +
                                   args.pop_front() + "'.");
    }
    do {
      if (*args.peek_front() == '-') break;
      value().push_back(args.pop_front());
    } while (args);
    return true;
  }
};

}  // namespace lyrahgames::options

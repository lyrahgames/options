#pragma once
#include <vector>
//
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

template <static_zstring N, static_zstring D>
struct attachment_list {
  using value_type = vector<czstring>;

  static constexpr auto name() { return N; }

  static constexpr auto description() { return D; }

  static constexpr auto help() {
    return static_zstring("--") + N + static_zstring(" <value> [<value> ...]");
  }

  constexpr operator bool() noexcept { return !value_.empty(); }

  constexpr auto value() noexcept -> value_type& { return value_; }

  constexpr auto value() const noexcept -> const value_type& { return value_; }

  constexpr bool parse(czstring call, arg_list& args) {
    if (strcmp(call, name())) return false;
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(args, string("No given value for option '") +
                                   args.pop_front() + "'.");
    }
    do {
      if (*args.peek_front() == '-') break;
      value_.push_back(args.pop_front());
    } while (args);
    return true;
  }

  value_type value_{};
};

}  // namespace lyrahgames::options

#pragma once
#include <lyrahgames/options/parser.hpp>

namespace lyrahgames::options {

// --flag value
template <static_zstring N, static_zstring D, char S = '\0'>
struct attachment : attachment<N, D, '\0'> {
  using base = attachment<N, D, '\0'>;
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
          args, std::string("No given value for short option '") + c + "'.");
    }
    val = args.pop_front();
    return true;
  }
};

template <static_zstring N, static_zstring D>
struct attachment<N, D, '\0'> {
  using value_type = czstring;
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator czstring() { return val; }
  static constexpr auto help() {
    return static_zstring("--") + N + static_zstring(" <value>");
  }
  constexpr auto value() noexcept -> value_type& { return val; }
  constexpr auto value() const noexcept -> value_type { return val; }
  constexpr bool parse(czstring call, arg_list& args) {
    if (std::strcmp(call, name())) return false;
    if (args.empty()) {
      args.unpop_front();
      throw parser_error(args, std::string("No given value for option '") +
                                   args.pop_front() + "'.");
    }
    val = args.pop_front();
    return true;
  }
  czstring val = "";
};

}  // namespace lyrahgames::options

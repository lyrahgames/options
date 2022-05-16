#pragma once
#include <lyrahgames/options/option_list.hpp>
#include <lyrahgames/options/utility.hpp>

namespace lyrahgames::options {

/// Standard Exception for Parsing Errors
/// It also provides the currently processed 'arg_list'.
/// So, after catching the exception
/// it can be used to go on with the parsing.
struct parser_error : std::exception {
  parser_error() = default;

  /// Constructor to provide list of arguments and an error message.
  parser_error(const arg_list& args,
               generic::forwardable<std::string> auto&& text)
      : state{args}, msg{std::forward<std::string>(text)} {}

  auto what() const noexcept -> czstring override { return msg.c_str(); }
  auto args() const noexcept -> const arg_list& { return state; }

 private:
  std::string msg{};
  arg_list state{};
};

/// Check for short option pattern.
/// -io, -i
constexpr auto no_short_flag_prefix(czstring str) noexcept -> czstring {
  if (*str++ != '-') return nullptr;
  if (*str == '-') return nullptr;
  return str;
}

/// Check for flag pattern.
/// --help
constexpr auto no_flag_prefix(czstring str) noexcept -> czstring {
  if (*str++ != '-') return nullptr;
  if (*str++ != '-') return nullptr;
  return str;
}

/// Algorithm to parse the short option patterns.
constexpr bool parse_short_options(arg_list& args,
                                   auto& options,
                                   czstring current) {
  // First, check if the given argument is actually a short option pattern.
  auto arg = no_short_flag_prefix(current);
  if (!arg) return false;
  // Iterate over all characters in the short option string.
  for (; *arg; ++arg) {
    // Push parsing of short option to all option types
    // until parsing can be applied by using 'for_each_until'.
    const auto index = for_each_until(options, [&](auto& option) {
      if constexpr (!generic::has_short_name<decltype(option)>)
        return false;
      else
        return option.parse(*arg, args);
    });
    // If no option provides this short name, throw an exception.
    if (index >= options.size()) {
      args.unpop_front();
      throw parser_error(args, std::string("Unknown short option '") + *arg +
                                   "' in '" + current + "'.");
    }
  }
  return true;
}

/// Algorithm to parse flag patterns.
constexpr bool parse_option(arg_list& args, auto& options, czstring current) {
  // Check if current argument is actually a flag.
  const auto call = no_flag_prefix(current);
  if (!call) return false;
  // Push the parsing the all the options until it is successful.
  const auto index =
      for_each_until(options, [&](auto& x) { return x.parse(call, args); });
  // Throw an exception if no option with the given name exists.
  if (index >= options.size()) {
    args.unpop_front();
    throw parser_error(args, std::string("Unknown option '") + current + "'.");
  }
  return true;
}

/// Function to parse the arguments provided by 'arg_list'
/// into a custom 'option_list' type.
/// The first argument will be assummed to be
/// the program name and therefore ignored.
constexpr void parse(arg_list args, auto& options) {
  // Assume that the first argument is the name of the program.
  args.pop_front();
  // Process all arguments in the list.
  while (!args.empty()) {
    const auto current = args.pop_front();
    // Short option pattern: -i, -io
    if (parse_short_options(args, options, current)) continue;
    // Flags: --input, --help
    if (parse_option(args, options, current)) continue;
    // Handle arguments that are no short option nor flag by an excption.
    args.unpop_front();
    throw parser_error(
        args, std::string("Given option '") + current + "' is not a flag.");
  }
}

}  // namespace lyrahgames::options

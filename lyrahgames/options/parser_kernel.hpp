#pragma once
#include <lyrahgames/options/option_parser/name_parser.hpp>
#include <lyrahgames/options/option_parser/positional_parser.hpp>
#include <lyrahgames/options/option_parser/short_name_parser.hpp>
#include <lyrahgames/options/parser_utility.hpp>

namespace lyrahgames::options {

constexpr auto default_parser_schedule() noexcept {
  return parser_schedule<named<"--", name_parser>,
                         named<"-", short_name_parser>>{};
}

template <typename position_schedule>
constexpr auto default_parser_schedule() noexcept {
  return parser_schedule<named<"--", name_parser>,
                         named<"-", short_name_parser>,
                         named<"", positional_parser<position_schedule>>>{};
}

/// Function to parse the arguments provided by 'arg_list'
/// into a custom 'option_list' type.
/// The first argument will be assummed to be
/// the program name and therefore ignored.
template <instance::option_list option_list,
          instance::parser_schedule<option_list> parser_schedule>
constexpr void parse(arg_list args,
                     option_list& opts,
                     parser_schedule schedule) {
  // Generate a static radix tree of prefixes based on the given prefix parsers.
  using prefixes = static_name_tree<parser_schedule>;
  using namespace static_radix_tree;
  // Assume that the first argument is the name of the program.
  args.pop_front();
  // Process all arguments in the list one after another.
  while (!args.empty()) {
    const auto current = args.pop_front();
    // Traverse the static prefix tree with the current argument
    // and call the specific parsing routine by template parameters
    // when a prefix could be matched.
    const auto prefix_matched =
        traverse<prefixes>(current, [&]<static_zstring prefix>(czstring tail) {
          get_by_name<prefix>(schedule).parse(tail, args, opts);
        });
    if (prefix_matched) continue;
    // If no prefix could be matched then we failed to parse the given argument.
    args.unpop_front();
    throw parser_error(
        args, string("Given option '") + args.front() + "' is not a flag.");
  }
}

constexpr void parse(arg_list args, instance::option_list auto& opts) {
  parse(args, opts, default_parser_schedule());
}

template <typename position_schedule>
constexpr void parse(arg_list args, instance::option_list auto& opts) {
  parse(args, opts, default_parser_schedule<position_schedule>());
}

}  // namespace lyrahgames::options

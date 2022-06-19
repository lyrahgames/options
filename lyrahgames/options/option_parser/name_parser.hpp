#pragma once
#include <lyrahgames/options/parser_utility.hpp>

namespace lyrahgames::options {

struct name_parser {
  template <instance::option_list options>
  static constexpr void parse(czstring current, arg_list& args, options& opts) {
    // Generate a static radix tree of option names based on the given option list.
    using names = name_tree<options>;
    using namespace static_radix_tree;
    // Try to match the names inside the radix tree as prefixes
    // and call in a successful case the respective parsing routine
    // of the specific option type.
    const auto prefix_matched =
        traverse<names>(current, [&]<static_zstring prefix>(czstring tail) {
          option<prefix>(opts).parse(tail, args);
        });
    if (prefix_matched) return;
    // If no name could be matched as prefix,
    // we have failed to parse the argument.
    args.unpop_front();
    throw parser_error(args, string("Unknown option '") + args.front() + "'.");
  }
};

}  // namespace lyrahgames::options

#pragma once
#include <lyrahgames/options/arg_list.hpp>
#include <lyrahgames/options/named_tuple.hpp>
#include <lyrahgames/options/option_list.hpp>
#include <lyrahgames/options/position_list.hpp>

namespace lyrahgames::options {

/// Standard Exception for Parsing Errors
/// It also provides the currently processed 'arg_list'.
/// So, after catching the exception
/// it can be used to go on with the parsing.
struct parser_error : exception {
  parser_error() = default;

  /// Constructor to provide list of arguments and an error message.
  parser_error(const arg_list& args, generic::forwardable<string> auto&& text)
      : state{args}, msg{forward<string>(text)} {}

  auto what() const noexcept -> czstring override { return msg.c_str(); }
  auto args() const noexcept -> const arg_list& { return state; }

 private:
  string msg{};
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
    const auto parsed = for_each_until(options, [&](auto& option) {
      if constexpr (!generic::has_short_name<decay_t<decltype(option)>>)
        return false;
      else
        return option.parse(*arg, args);
    });
    // If no option provides this short name, throw an exception.
    if (!parsed) {
      args.unpop_front();
      throw parser_error(args, string("Unknown short option '") + *arg +
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
  const auto parsed =
      for_each_until(options, [&](auto& x) { return x.parse(call, args); });
  // Throw an exception if no option with the given name exists.
  if (!parsed) {
    args.unpop_front();
    throw parser_error(args, string("Unknown option '") + current + "'.");
  }
  return true;
}

template <typename list>
constexpr bool parse_position(arg_list& args,
                              auto& options,
                              czstring current,
                              size_t position) {
  return false;
}

template <typename list>
constexpr void parse(arg_list args, auto& options) {
  // Assume that the first argument is the name of the program.
  args.pop_front();
  size_t position = 0;
  // Process all arguments in the list.
  while (!args.empty()) {
    const auto current = args.pop_front();
    // Short option pattern: -i, -io
    if (parse_short_options(args, options, current)) continue;
    // Flags: --input, --help
    if (parse_option(args, options, current)) continue;
    // Handle arguments that are no short option nor flag by an excption.

    ++position;
    // parse_position<list>(args, options, current, position);
    const auto visited = visit<list>(position, [&]<static_zstring name> {
      option<name>(options).parse(current, args, position);
    });
    if (!visited) {
      args.unpop_front();
      throw parser_error(
          args, string("Given option '") + current + "' is not a flag.");
    }
  }
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
    throw parser_error(args,
                       string("Given option '") + current + "' is not a flag.");
  }
}

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
          option<prefix>(opts).tree_parse(tail, args);
        });
    if (prefix_matched) return;
    // If no name could be matched as prefix,
    // we have failed to parse the argument.
    args.unpop_front();
    throw parser_error(args, string("Unknown option '") + args.front() + "'.");
  }
};

struct short_name_parser {
  template <instance::option_list options>
  static constexpr void parse(czstring current, arg_list& args, options& opts) {
    for (auto arg = current; *arg; ++arg) {
      const auto parsed = for_each_until(opts, [&](auto& opt) {
        if constexpr (!generic::has_short_name2<decay_t<decltype(opt)>>)
          return false;
        else {
          if (opt.short_name() != *arg) return false;
          opt.tree_parse(args);
          return true;
        }
      });
      if (parsed) continue;
      args.unpop_front();
      throw parser_error(args, string("Unknown short option '") + *arg +
                                   "' in '" + args.front() + "'.");
    }
  }
};

template <typename position_schedule>
struct positional_parser {
  template <instance::option_list options>
  constexpr void parse(czstring current, arg_list& args, options& opts) {
    ++position;
    const auto visited =
        visit<position_schedule>(position, [&]<static_zstring name> {
          option<name>(opts).parse(current, args, position);
        });
    if (visited) return;
    args.unpop_front();
    throw parser_error(args, string("Failed to parse positional argument '") +
                                 args.front() + "' due to wrong positioning.");
  }

  size_t position = 0;
};

namespace generic {
template <typename parser, typename options>
concept named_parser = named<parser> && instance::option_list<options> &&
    requires(parser p, czstring current, arg_list& args, options& opts) {
  { p.parse(current, args, opts) } -> same_as<void>;
};
}  // namespace generic

namespace detail {
template <instance::named_tuple schedule, instance::option_list options>
struct is_parser_schedule : false_type {};
template <instance::option_list options, typename... parsers>
struct is_parser_schedule<named_tuple<parsers...>, options> {
  static constexpr bool value =
      (generic::named_parser<parsers, options> && ...);
};
}  // namespace detail
template <instance::named_tuple schedule, instance::option_list options>
constexpr bool is_parser_schedule =
    detail::is_parser_schedule<schedule, options>::value;

namespace instance {
template <typename schedule, typename options>
concept parser_schedule = is_parser_schedule<schedule, options>;
}

template <generic::named... parsers>
using parser_schedule = named_tuple<parsers...>;

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

constexpr void fast_parse(arg_list args, instance::option_list auto& opts) {
  parse(args, opts, default_parser_schedule());
}

template <typename position_schedule>
constexpr void fast_parse(arg_list args, instance::option_list auto& opts) {
  parse(args, opts, default_parser_schedule<position_schedule>());
}

}  // namespace lyrahgames::options

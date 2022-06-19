#pragma once
#include <lyrahgames/options/parser_utility.hpp>

namespace lyrahgames::options {

namespace generic {

template <typename option_type>
concept short_name_parsable = requires(option_type& option, arg_list& args) {
  { option_type::short_name() } -> same_as<char>;
  option.parse(args);
};

}  // namespace generic

struct short_name_parser {
  template <instance::option_list options>
  static constexpr void parse(czstring current, arg_list& args, options& opts) {
    auto args_backup = args;
    for (auto arg = current; *arg; ++arg) {
      const auto parsed =
          for_each_until(opts, [&]<typename option_type>(option_type& option) {
            if constexpr (generic::short_name_parsable<option_type>) {
              if (option_type::short_name() != *arg) return false;
              option.parse(args);
              return true;
            } else
              return false;
          });
      if (parsed) continue;
      args_backup.unpop_front();
      throw parser_error(args_backup, string("Unknown short option '") + *arg +
                                          "' in '" + args_backup.front() +
                                          "'.");
    }
  }
};

}  // namespace lyrahgames::options

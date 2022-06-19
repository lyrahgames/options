#pragma once
#include <lyrahgames/options/parser_utility.hpp>

namespace lyrahgames::options {

struct short_name_parser {
  template <instance::option_list options>
  static constexpr void parse(czstring current, arg_list& args, options& opts) {
    for (auto arg = current; *arg; ++arg) {
      const auto parsed = for_each_until(opts, [&](auto& opt) {
        if constexpr (!generic::has_short_name2<decay_t<decltype(opt)>>)
          return false;
        else {
          if (opt.short_name() != *arg) return false;
          opt.parse(args);
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

}  // namespace lyrahgames::options

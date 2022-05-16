#pragma once
#include <lyrahgames/options/option_list.hpp>
#include <lyrahgames/options/utility.hpp>

namespace lyrahgames::options {

struct parse_error : std::exception {
  parse_error() = default;
  parse_error(const arg_list& args,
              generic::forwardable<std::string> auto&& text)
      : state{args}, msg{std::forward<std::string>(text)} {}

  auto what() const noexcept -> czstring override { return msg.c_str(); }
  auto args() const noexcept -> const arg_list& { return state; }

 private:
  std::string msg{};
  arg_list state{};
};

constexpr auto no_short_flag_prefix(czstring str) noexcept -> czstring {
  if (*str++ != '-') return nullptr;
  if (*str == '-') return nullptr;
  return str;
}

constexpr auto no_flag_prefix(czstring str) noexcept -> czstring {
  if (*str++ != '-') return nullptr;
  if (*str++ != '-') return nullptr;
  return str;
}

template <typename... types>
constexpr void parse(arg_list args, option_list<types...>& options) {
  args.pop_front();
  while (!args.empty()) {
    const auto current = args.pop_front();

    auto arg = no_short_flag_prefix(current);
    if (arg) {
      for (; *arg; ++arg) {
        const auto index = for_each_until(options, [&](auto& option) {
          if constexpr (!generic::has_short_name<decltype(option)>)
            return false;
          else
            return option.parse(*arg, args);
        });
        if (index >= options.size()) {
          args.unpop_front();
          throw parse_error(args, std::string("Unknown short option '") + *arg +
                                      "' in '" + current + "'.");
        }
      }
      continue;
    }

    const auto call = no_flag_prefix(current);
    if (!call) {
      args.unpop_front();
      throw parse_error(
          args, std::string("Given option '") + current + "' is not a flag.");
    }
    const auto index =
        for_each_until(options, [&](auto& x) { return x.parse(call, args); });
    if (index >= options.size()) {
      args.unpop_front();
      throw parse_error(args, std::string("Unknown option '") + current + "'.");
    }
  }
}

}  // namespace lyrahgames::options

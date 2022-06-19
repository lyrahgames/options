#pragma once
#include <lyrahgames/options/parser_utility.hpp>

namespace lyrahgames::options {

template <size_t N>
struct position {
  constexpr position(const char (&n)[N], size_t c = 1) : name{n}, count{c} {}
  constexpr position(static_zstring<N> n, size_t c = 1) : name{n}, count{c} {}

  static_zstring<N> name{};
  size_t count{};
};

constexpr size_t undefined = -1;

template <position... p>
struct position_list {
  static constexpr auto size() noexcept -> size_t { return (p.count + ...); }
};

template <position p, position... q>
constexpr bool visit(size_t x, auto&& f, size_t last = 0) {
  if constexpr (p.count == undefined) {
    forward<decltype(f)>(f).template operator()<p.name>();
    return true;
  } else {
    last += p.count;
    if (x <= last) {
      forward<decltype(f)>(f).template operator()<p.name>();
      return true;
    }
    if constexpr (sizeof...(q))
      return visit<q...>(x, forward<decltype(f)>(f), last);
    else
      return false;
  }
}

template <typename T>
struct visit_helper;

template <position... p>
struct visit_helper<position_list<p...>> {
  constexpr bool operator()(size_t x, auto&& f) {
    return visit<p...>(x, forward<decltype(f)>(f));
  }
};

template <typename T>
constexpr bool visit(size_t x, auto&& f) {
  return visit_helper<T>{}(x, forward<decltype(f)>(f));
}

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

}  // namespace lyrahgames::options

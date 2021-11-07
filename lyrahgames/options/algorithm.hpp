#pragma once
#include <lyrahgames/options/option_list.hpp>

namespace lyrahgames::options {

template <size_t N, typename function, typename... types>
requires(N >= sizeof...(types))  //
    constexpr void for_each(option_list<types...>& options, function&& f) {}

template <size_t N = 0, typename function, typename... types>
requires(N < sizeof...(types))  //
    constexpr void for_each(option_list<types...>& options, function&& f) {
  f(get<N>(options));
  for_each<N + 1>(options, std::forward<function>(f));
}

template <size_t N, typename function, typename... types>
requires(N >= sizeof...(types))  //
    constexpr auto for_each_until(option_list<types...>& options, function&& f)
        -> size_t {
  return sizeof...(types);
}

template <size_t N = 0, typename function, typename... types>
requires(N < sizeof...(types))  //
    constexpr auto for_each_until(option_list<types...>& options, function&& f)
        -> size_t {
  if (f(get<N>(options))) return N;
  return for_each_until<N + 1>(options, std::forward<function>(f));
}

}  // namespace lyrahgames::options

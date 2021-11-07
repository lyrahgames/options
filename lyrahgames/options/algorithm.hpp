#pragma once
#include <lyrahgames/options/option_list.hpp>

namespace lyrahgames::options {

/// Algorithm for iterating statically over every option in an option list
/// and applying the given function object.
// Sentinel function at the end which is doing nothing.
template <size_t N, typename function, typename... types>
requires(N >= sizeof...(types))  //
    constexpr void for_each(option_list<types...>& options, function&& f) {}
// This function applies the function object to the current option
// and afterwards goes on with the loop for all tail options.
template <size_t N = 0, typename function, typename... types>
requires(N < sizeof...(types))  //
    constexpr void for_each(option_list<types...>& options, function&& f) {
  f(get<N>(options));
  for_each<N + 1>(options, std::forward<function>(f));
}

/// Algorithm for iterating statically over every option in an option list
/// and applying the given function object until it returns true for the first time.
/// The algorithm returns the index at which it stopped.
// Sentinel function returning end index.
template <size_t N, typename function, typename... types>
requires(N >= sizeof...(types))  //
    constexpr auto for_each_until(option_list<types...>& options, function&& f)
        -> size_t {
  return sizeof...(types);
}
// Calls function object on current option and checks output.
// If false, go on with the loop.
template <size_t N = 0, typename function, typename... types>
requires(N < sizeof...(types))  //
    constexpr auto for_each_until(option_list<types...>& options, function&& f)
        -> size_t {
  if (f(get<N>(options))) return N;
  return for_each_until<N + 1>(options, std::forward<function>(f));
}

}  // namespace lyrahgames::options

#pragma once
#include <lyrahgames/xstd/forward.hpp>
//
#include <lyrahgames/options/option.hpp>

namespace lyrahgames::options {

// Creating your own option_list provides advantages instead of using std::tuple.
// First, ADL is enabled for get and value routine.
template <generic::option...>
struct option_list;

template <>
struct option_list<> {
  static constexpr auto size() noexcept -> size_t { return 0; }

  constexpr option_list() = default;

  template <size_t N>
  constexpr auto option() const = delete;

  template <static_zstring N>
  constexpr auto option() const = delete;
};

template <generic::option T, generic::option... U>
struct option_list<T, U...> : T, option_list<U...> {
  using head = T;
  using next = option_list<U...>;

  static constexpr auto size() noexcept { return 1 + sizeof...(U); }

  constexpr option_list() = default;

  template <generic::forwardable<head> first,
            generic::forwardable<next>... tail>
  constexpr option_list(first&& f, tail&&... t)
      : head{std::forward<first>(f)}, next{std::forward<tail>(t)...} {}

  template <size_t index>
  requires(index == 0)  //
      constexpr decltype(auto) option() {
    return static_cast<head&>(*this);
  }
  template <size_t index>
  requires((index != 0) && (index < size()))  //
      constexpr decltype(auto) option() {
    return static_cast<next&>(*this).template option<index - 1>();
  }

  template <static_zstring str>
  requires(str == head::name())  //
      constexpr decltype(auto) option() const {
    return static_cast<const head&>(*this);
  }
  template <static_zstring str>
  requires(str != head::name())  //
      constexpr decltype(auto) option() const {
    return static_cast<const next&>(*this).template option<str>();
  }
};

template <typename... types>
option_list(types&&... x) -> option_list<types...>;

template <size_t index, typename... types>
constexpr decltype(auto) get(option_list<types...>& options) {
  return options.template option<index>();
}

template <static_zstring str, typename... types>
constexpr decltype(auto) get(const option_list<types...>& options) {
  return options.template option<str>();
}

template <size_t index, typename... types>
constexpr decltype(auto) option(option_list<types...>& options) {
  return options.template option<index>();
}

template <static_zstring str, typename... types>
constexpr decltype(auto) option(const option_list<types...>& options) {
  return options.template option<str>();
}

template <size_t index, typename... types>
constexpr decltype(auto) value(option_list<types...>& options) {
  return options.template option<index>().value();
}

template <static_zstring str, typename... types>
constexpr decltype(auto) value(const option_list<types...>& options) {
  return options.template option<str>().value();
}

}  // namespace lyrahgames::options

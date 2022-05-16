#pragma once
#include <lyrahgames/options/option.hpp>

namespace lyrahgames::options {

// We need a type-dependent expression that is always false
// to provide useful compile-time error messages with static_assert.
template <static_zstring name>
constexpr bool unknown = false;

// Creating your own option_list provides advantages instead of using std::tuple.
// First, ADL is enabled for get and value routine.

template <generic::option...>
struct option_list;

// Sentinel specialization of option list.
// For now, we use deleted functions to say
// that the given name or index is not valid.
template <>
struct option_list<> {
  constexpr option_list() = default;

  template <size_t index>
  constexpr auto option() const noexcept = delete;

  template <static_zstring name>
  constexpr auto option() const noexcept = delete;
  // constexpr decltype(auto) option() const {
  //   static_assert(unknown<name>, "Unknown option name");
  //   return *this;
  // }

  static constexpr auto size() noexcept -> size_t { return 0; }
};

// Actual implementation of the list of options.
template <generic::option T, generic::option... U>
struct option_list<T, U...> : T, option_list<U...> {
  using head = T;
  using next = option_list<U...>;

  constexpr option_list() = default;

  // Most general forward constructor
  template <generic::forwardable<head> first,
            generic::forwardable<next>... tail>
  constexpr option_list(first&& f, tail&&... t)
      : head{std::forward<first>(f)}, next{std::forward<tail>(t)...} {}

  static constexpr auto size() noexcept { return 1 + sizeof...(U); }

  /// Non-const member function for accessing a specific option by its index.
  template <size_t index>
  constexpr decltype(auto) option() noexcept {
    // Using constexpr if instead of constraints and concepts
    // keeps the implementation short.
    if constexpr (index == 0)
      return static_cast<head&>(*this);
    else
      return static_cast<next&>(*this).template option<index - 1>();
  }

  /// Const member function for accessing a specific option by its index.
  template <size_t index>
  constexpr const auto& option() const noexcept {
    // We do not want to copy source code.
    // Therefore we use a const cast
    //to call the already implemented alternative.
    return const_cast<option_list&>(*this).template option<index>();
  }

  /// Non-const member function for accessing a specific option by its name.
  template <static_zstring name>
  constexpr decltype(auto) option() noexcept {
    // Using constexpr if instead of constraints and concepts
    // keeps the implementation short.
    if constexpr (name == head::name())
      return static_cast<head&>(*this);
    else
      return static_cast<next&>(*this).template option<name>();
  }

  /// Const member function for accessing a specific option by its name.
  template <static_zstring name>
  constexpr const auto& option() const noexcept {
    // We do not want to copy source code.
    // Therefore we use a const cast
    //to call the already implemented alternative.
    return const_cast<option_list&>(*this).template option<name>();
  }
};

template <typename... types>
option_list(types&&... x) -> option_list<types...>;

/// Helper function to access an option
/// of a list of options directly by its index.
template <size_t index>
constexpr decltype(auto) option(auto&& options) {
  return std::forward<decltype(options)>(options).template option<index>();
}

/// Helper function to access an option
/// of a list of options directly by its name.
template <static_zstring name>
constexpr decltype(auto) option(auto&& options) {
  return std::forward<decltype(options)>(options).template option<name>();
}

/// Helper function to access the value of an option
/// inside a list of options by its index.
template <size_t index>
constexpr decltype(auto) value(auto&& options) {
  return std::forward<decltype(options)>(options)
      .template option<index>()
      .value();
}

/// Helper function to access the value of an option
/// inside a list of options by its name.
template <static_zstring name>
constexpr decltype(auto) value(auto&& options) {
  return std::forward<decltype(options)>(options)
      .template option<name>()
      .value();
}

}  // namespace lyrahgames::options

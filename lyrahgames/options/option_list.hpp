#pragma once
#include <lyrahgames/options/arg_list.hpp>

namespace lyrahgames::options {

namespace generic {

/// Generic option concept for option types listed inside 'option_list'.
template <typename T>
concept option = requires(T& x, const T& c, czstring call, arg_list& args) {
  { T::name() } -> convertible_to<czstring>;
  { T::description() } -> convertible_to<czstring>;
  { x.value() } -> identical<typename T::value_type&>;
  { c.value() } -> convertible_to<typename T::value_type>;
  { x.parse(call, args) } -> convertible_to<bool>;
};

template <typename T>
concept has_short_name = requires(T x) {
  { x.short_name() } -> same_as<char>;
};

}  // namespace generic

// We need a type-dependent expression that is always false
// to provide useful compile-time error messages with static_assert.
template <static_zstring name>
constexpr bool unknown = false;

// We need a type-dependent expression that is always false
// to provide useful compile-time error messages with static_assert.
template <size_t index>
constexpr bool out_of_bounds = false;

/// Sentinel type for better compile-time error messages.
struct invalid_option {
  constexpr bool value() { return false; }
};

// Creating your own option_list provides advantages instead of using tuple.
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
  // constexpr auto option() const noexcept = delete;
  constexpr auto option() const {
    static_assert(out_of_bounds<index>, "Given option index out of bounds");
    // Accessing the value of option provides other not useful
    // error messages besides from the static assertion.
    // Using this sentinel type with a value member function is a workaround.
    return invalid_option{};
  }

  template <static_zstring name>
  // constexpr auto option() const noexcept = delete;
  constexpr auto option() const {
    static_assert(unknown<name>, "Unknown option name");
    // Accessing the value of option provides other not useful
    // error messages besides from the static assertion.
    // Using this sentinel type with a value member function is a workaround.
    return invalid_option{};
  }

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
      : head{forward<first>(f)}, next{forward<tail>(t)...} {}

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
  return forward<decltype(options)>(options).template option<index>();
}

/// Helper function to access an option
/// of a list of options directly by its name.
template <static_zstring name>
constexpr decltype(auto) option(auto&& options) {
  return forward<decltype(options)>(options).template option<name>();
}

/// Helper function to access the value of an option
/// inside a list of options by its index.
template <size_t index>
constexpr decltype(auto) value(auto&& options) {
  return forward<decltype(options)>(options).template option<index>().value();
}

/// Helper function to access the value of an option
/// inside a list of options by its name.
template <static_zstring name>
constexpr decltype(auto) value(auto&& options) {
  return forward<decltype(options)>(options).template option<name>().value();
}

/// Algorithm for iterating statically over every option in an option list
/// and applying the given function object.
template <size_t index = 0>
constexpr void for_each(auto&& options, auto&& functor) {
  if constexpr (index < decay_t<decltype(options)>::size()) {
    functor(option<index>(options));
    for_each<index + 1>(forward<decltype(options)>(options),
                        forward<decltype(functor)>(functor));
  }
}

/// Algorithm for iterating statically over every option in an option list
/// and applying the given function object until it returns true for the first time.
/// The algorithm returns the index at which it stopped.
template <size_t index = 0>
constexpr auto for_each_until(auto&& options, auto&& functor) -> size_t {
  if constexpr (index < decay_t<decltype(options)>::size()) {
    if (functor(option<index>(options))) return index;
    return for_each_until<index + 1>(forward<decltype(options)>(options),
                                     forward<decltype(functor)>(functor));
  }
  return index;
}

}  // namespace lyrahgames::options

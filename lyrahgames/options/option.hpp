#pragma once
#include <cstring>
//
#include <lyrahgames/options/arg_list.hpp>

namespace lyrahgames::options {

namespace generic {
using namespace lyrahgames::xstd::generic;

template <typename T>
concept option = requires(T& x, const T& c, czstring call, arg_list& args) {
  { T::name() } -> std::convertible_to<czstring>;
  { T::description() } -> std::convertible_to<czstring>;
  { x.value() } -> identical<typename T::value_type&>;
  { c.value() } -> std::convertible_to<typename T::value_type>;
  { x.parse(call, args) } -> std::convertible_to<bool>;
};

template <typename T>
concept has_short_name = requires(T x) {
  { x.short_name() } -> std::same_as<char>;
};

}  // namespace generic

// --flag
template <static_zstring N, static_zstring D, char S = '\0'>
struct flag;

template <static_zstring N, static_zstring D>
struct flag<N, D, '\0'> {
  using value_type = bool;
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator bool() { return val; }
  constexpr auto help() { return static_zstring("--") + N; }
  constexpr auto value() noexcept -> value_type& { return val; }
  constexpr auto value() const noexcept -> value_type { return val; }
  constexpr bool parse(czstring call, arg_list& args) {
    if (std::strcmp(call, name())) return false;
    return (val = true);
  }
  bool val{false};
};

template <static_zstring N, static_zstring D, char S>
requires(S != '\0')  //
    struct flag<N, D, S> : flag<N, D, '\0'> {
  using base = flag<N, D, '\0'>;
  using base::description;
  using base::name;
  using base::parse;
  using base::val;
  using base::value;
  using base::value_type;
  static constexpr auto short_name() { return S; }
  constexpr auto help() {
    return static_zstring("-") + S + static_zstring(", ") + base::help();
  }
  constexpr bool parse(char c, arg_list& args) {
    if (c != short_name()) return false;
    return (val = true);
  }
};

// --flag value
template <static_zstring N, static_zstring D, char S = '\0'>
struct attachment : attachment<N, D, '\0'> {
  using base = attachment<N, D, '\0'>;
  using base::description;
  using base::name;
  using base::parse;
  using base::val;
  using base::value;
  using base::value_type;
  static constexpr auto short_name() noexcept { return S; }
  constexpr auto help() {
    return static_zstring("-") + S + static_zstring(", ") + base::help();
  }
  constexpr bool parse(char c, arg_list& args) {
    if (c != short_name()) return false;
    if (args.empty()) {
      throw std::invalid_argument(
          std::string("No given value for short option '") + c + "'.");
    }
    val = args.pop_front();
    return true;
  }
};

template <static_zstring N, static_zstring D>
struct attachment<N, D, '\0'> {
  using value_type = czstring;
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator czstring() { return val; }
  constexpr auto help() {
    return static_zstring("--") + N + static_zstring(" <value>");
  }
  constexpr auto value() noexcept -> value_type& { return val; }
  constexpr auto value() const noexcept -> value_type { return val; }
  constexpr bool parse(czstring call, arg_list& args) {
    if (std::strcmp(call, name())) return false;
    if (args.empty()) {
      args.unpop_front();
      throw std::invalid_argument(std::string("No given value for option '") +
                                  args.pop_front() + "'.");
    }
    val = args.pop_front();
    return true;
  }
  czstring val = "";
};

// --key=value
template <static_zstring N, static_zstring D>
struct assignment {
  using value_type = czstring;
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator czstring() { return val; }
  constexpr auto help() {
    return static_zstring("--") + N + static_zstring("=VALUE");
  }
  constexpr auto value() noexcept -> value_type& { return val; }
  constexpr auto value() const noexcept -> value_type { return val; }
  constexpr bool parse(czstring call, arg_list& args) {
    const auto tmp = name();  // Need this one due to optimization.
    czstring n = tmp;
    while (*n && *call && (*n == *call)) ++n, ++call;
    if (!*n && !*call) {
      args.unpop_front();
      throw std::invalid_argument(std::string("No assignment for option '") +
                                  args.pop_front() + "'.");
    }
    if (*n) return false;
    if (*call != '=') return false;
    val = call + 1;
    return true;
  }
  czstring val = "";
};

}  // namespace lyrahgames::options

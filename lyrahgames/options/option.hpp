#pragma once
#include <cstring>
//
#include <lyrahgames/options/arg_list.hpp>

namespace lyrahgames::options {

namespace generic {
using namespace lyrahgames::xstd::generic;

template <typename T>
concept option = requires(T x) {
  { T::name() } -> std::convertible_to<czstring>;
  { T::description() } -> std::convertible_to<czstring>;
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
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator bool() { return value; }
  constexpr auto help() { return static_zstring("--") + N; }
  constexpr bool parse(czstring call, arg_list& args) {
    if (std::strcmp(call, name())) return false;
    return (value = true);
  }
  bool value{false};
};

template <static_zstring N, static_zstring D, char S>
requires(S != '\0')  //
    struct flag<N, D, S> : flag<N, D, '\0'> {
  using base = flag<N, D, '\0'>;
  using base::description;
  using base::name;
  using base::parse;
  using base::value;
  static constexpr auto short_name() { return S; }
  constexpr auto help() {
    return static_zstring("-") + S + static_zstring(", ") + base::help();
  }
  constexpr bool parse(char c, arg_list& args) {
    if (c != short_name()) return false;
    return (value = true);
  }
};

// --flag value
template <static_zstring N, static_zstring D, char S = '\0'>
struct attachment : attachment<N, D, '\0'> {
  using base = attachment<N, D, '\0'>;
  using base::description;
  using base::name;
  using base::parse;
  using base::value;
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
    value = args.pop_front();
    return true;
  }
};

template <static_zstring N, static_zstring D>
struct attachment<N, D, '\0'> {
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator czstring() { return value; }
  constexpr auto help() {
    return static_zstring("--") + N + static_zstring(" <value>");
  }
  constexpr bool parse(czstring call, arg_list& args) {
    if (std::strcmp(call, name())) return false;
    if (args.empty()) {
      args.unpop_front();
      throw std::invalid_argument(std::string("No given value for option '") +
                                  args.pop_front() + "'.");
    }
    value = args.pop_front();
    return true;
  }
  czstring value = "";
};

// --key=value
template <static_zstring N, static_zstring D>
struct assignment {
  static constexpr auto name() { return N; }
  static constexpr auto description() { return D; }
  constexpr operator czstring() { return value; }
  constexpr auto help() {
    return static_zstring("--") + N + static_zstring("=VALUE");
  }
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
    value = call + 1;
    return true;
  }
  czstring value = "";
};

}  // namespace lyrahgames::options

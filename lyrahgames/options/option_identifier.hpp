#pragma once
#include <lyrahgames/options/utility.hpp>

namespace lyrahgames::options {

template <size_t N, bool = false>
struct option_identifier {
  constexpr option_identifier(const char (&n)[N]) noexcept : name_{n} {}
  constexpr option_identifier(static_zstring<N> n) noexcept : name_{n} {}
  constexpr auto name() const noexcept { return name_; }
  static constexpr bool has_short_name() noexcept { return false; }
  static_zstring<N> name_;
};

template <size_t N>
struct option_identifier<N, true> : option_identifier<N, false> {
  using base = option_identifier<N, false>;
  constexpr option_identifier(const char (&n)[N], char s) noexcept
      : base(n), short_name_{s} {}
  constexpr option_identifier(static_zstring<N> n, char s) noexcept
      : base(n), short_name_{s} {}
  static constexpr bool has_short_name() noexcept { return true; }
  constexpr char short_name() const noexcept { return short_name_; }
  char short_name_;
};

template <size_t N>
option_identifier(const char (&)[N]) -> option_identifier<N, false>;
template <size_t N>
option_identifier(static_zstring<N>) -> option_identifier<N, false>;
template <size_t N>
option_identifier(const char (&)[N], char) -> option_identifier<N, true>;
template <size_t N>
option_identifier(static_zstring<N>, char) -> option_identifier<N, true>;

}  // namespace lyrahgames::options

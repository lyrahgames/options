#pragma once
#include <cstdint>

namespace lyrahgames::options {

using zstring = char*;
using czstring = const char*;

template <size_t N>
struct static_zstring {
  static constexpr auto size() -> size_t { return N; }

  constexpr static_zstring() noexcept = default;
  constexpr static_zstring(const char (&str)[N]) noexcept {
    for (size_t i = 0; i < N; ++i) data[i] = str[i];
  }

  constexpr operator zstring() noexcept { return data; }
  constexpr operator czstring() const noexcept { return data; }

  friend constexpr auto operator<=>(const static_zstring&,
                                    const static_zstring&) noexcept = default;

  char data[N]{};
};

template <size_t N>
constexpr auto operator+(static_zstring<N> x, char c) noexcept {
  static_zstring<N + 1> result{};
  for (size_t i = 0; i < N - 1; ++i) result.data[i] = x.data[i];
  result.data[N - 1] = c;
  return result;
}

template <size_t N, size_t M>
constexpr auto operator+(static_zstring<N> x, static_zstring<M> y) noexcept {
  static_zstring<N + M - 1> result{};
  size_t i = 0;
  for (; i < N - 1; ++i) result.data[i] = x.data[i];
  for (; i < N + M - 1; ++i) result.data[i] = y.data[i - N + 1];
  return result;
}

}  // namespace lyrahgames::options

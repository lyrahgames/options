#pragma once
#include <lyrahgames/options/utility.hpp>

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

}  // namespace lyrahgames::options

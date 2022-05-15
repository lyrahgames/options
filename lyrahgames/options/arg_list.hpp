#pragma once
#include <lyrahgames/options/utility.hpp>

namespace lyrahgames::options {

struct arg_list {
  size_t count{};
  czstring* values{};
  size_t index{};

  constexpr arg_list() = default;
  constexpr arg_list(int argc, czstring* argv, int i = 0)
      : count(argc), values{argv}, index(i) {}
  constexpr arg_list(int argc, char** argv, int i = 0)
      : count(argc), values{const_cast<czstring*>(argv)}, index(i) {}

  constexpr auto operator[](size_t i) const noexcept -> czstring {
    return values[index + i];
  }
  constexpr auto size() const noexcept -> size_t { return count - index; }
  constexpr bool empty() const noexcept { return index >= count; }

  constexpr auto pop_front() noexcept -> czstring { return values[index++]; }
  constexpr void unpop_front() noexcept { --index; }
  constexpr auto peek_front() const noexcept -> czstring {
    return values[index];
  }
};

}  // namespace lyrahgames::options

#pragma once
#include <lyrahgames/options/option_identifier.hpp>

namespace lyrahgames::options {

template <typename T, option_identifier N, static_zstring D>
struct basic_option {
  using value_type = T;

  /// Provide static C string for the name of the option.
  static constexpr auto name() noexcept { return N.name(); }
  /// Provide static C string for the description of the option.
  static constexpr auto description() noexcept { return D; }

  static constexpr bool has_short_name() noexcept { return N.has_short_name(); }

  static constexpr auto short_name() noexcept requires(has_short_name()) {
    return N.short_name();
  }

  /// Allow to access the value as reference.
  constexpr auto value() noexcept -> value_type& { return value_; }
  constexpr auto value() const noexcept -> const value_type& { return value_; }

  value_type value_{};
};

}  // namespace lyrahgames::options

#pragma once
#include <lyrahgames/options/arg_list.hpp>
#include <lyrahgames/options/named_tuple.hpp>
#include <lyrahgames/options/option_list.hpp>
#include <lyrahgames/options/utility.hpp>

namespace lyrahgames::options {

/// Standard Exception for Parsing Errors
/// It also provides the currently processed 'arg_list'.
/// So, after catching the exception,
/// it can be used to go on with the parsing.
struct parser_error : exception {
  parser_error() = default;

  /// Constructor to provide list of arguments and an error message.
  parser_error(const arg_list& args, generic::forwardable<string> auto&& text)
      : state{args}, msg{forward<string>(text)} {}

  auto what() const noexcept -> czstring override { return msg.c_str(); }
  auto args() const noexcept -> const arg_list& { return state; }

 private:
  string msg{};
  arg_list state{};
};

namespace generic {
template <typename parser, typename options>
concept named_parser = named<parser> && instance::option_list<options> &&
    requires(parser p, czstring current, arg_list& args, options& opts) {
  { p.parse(current, args, opts) } -> same_as<void>;
};
}  // namespace generic

namespace detail {
template <instance::named_tuple schedule, instance::option_list options>
struct is_parser_schedule : false_type {};
template <instance::option_list options, typename... parsers>
struct is_parser_schedule<named_tuple<parsers...>, options> {
  static constexpr bool value =
      (generic::named_parser<parsers, options> && ...);
};
}  // namespace detail
template <instance::named_tuple schedule, instance::option_list options>
constexpr bool is_parser_schedule =
    detail::is_parser_schedule<schedule, options>::value;

namespace instance {
template <typename schedule, typename options>
concept parser_schedule = is_parser_schedule<schedule, options>;
}

template <generic::named... parsers>
using parser_schedule = named_tuple<parsers...>;

}  // namespace lyrahgames::options

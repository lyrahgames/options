#pragma once
#include <tuple>
//
#include <lyrahgames/options/utility.hpp>

namespace lyrahgames::options {

namespace generic {

template <typename type>
concept named = is_static_zstring(type::name);

template <typename type>
concept nameable = is_class_v<type> && !is_final_v<type>;

}  // namespace generic

template <static_zstring str, generic::nameable type>
struct named : type {
  static constexpr static_zstring name = str;
  using type::type;
  constexpr named(const type& p) : type(p) {}
  constexpr named(type&& p) : type(move(p)) {}
};

template <static_zstring name, generic::nameable type>
constexpr auto auto_named(type&& value) {
  return named<name, type>{forward<type>(value)};
}

namespace detail {
template <typename T>
struct is_named_tuple : false_type {};
template <generic::named... types>
struct is_named_tuple<std::tuple<types...>> : true_type {};
}  // namespace detail
template <typename T>
constexpr bool is_named_tuple = detail::is_named_tuple<T>::value;

template <generic::named... types>
using named_tuple = std::tuple<types...>;

namespace instance {
template <typename T>
concept named_tuple = is_named_tuple<T>;
}

template <static_zstring name, typename T, size_t index = 0>
requires instance::named_tuple<decay_t<T>>  //
constexpr decltype(auto) get_by_name(T&& t) noexcept {
  if constexpr (name == decay_t<decltype(get<index>(t))>::name)
    return get<index>(forward<T>(t));
  else
    return get_by_name<name, T, index + 1>(forward<T>(t));
}

namespace detail {
template <instance::named_tuple T>
struct static_name_tree {};
template <generic::named... types>
struct static_name_tree<std::tuple<types...>> {
  using type = static_radix_tree::construction<types::name...>;
};
}  // namespace detail
template <instance::named_tuple T>
using static_name_tree = typename detail::static_name_tree<T>::type;

}  // namespace lyrahgames::options

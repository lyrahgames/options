#include <iomanip>
#include <iostream>
//
#include <lyrahgames/options/options.hpp>

using namespace std;
using namespace lyrahgames;

namespace application {
using namespace options;
option_list<  //
    // new_flag<{"help", 'h'}, "Print the help message.">,
    flag<{"help", 'h'}, "Print the help message.">,
    flag<"version", "Print the help message.">,
    flag<{"quiet", 'q'}, "Print the help message.">,
    attachment<{"input", 'i'}, "Provide program input.">,
    attachment<{"output", 'o'}, "Provide program output.">,
    attachment<"type", "Provide program type.">,
    assignment<"key", "Provide a key.">>
    options{};
using positioning = position_list<"output", {"input", undefined}>;
}  // namespace application

void print_options() {
  for_each(application::options, [](const auto& x) {
    cout << left << setw(20) << x.help() << '\n'
         << '\t' << setw(40) << x.description() << boolalpha << setw(8);
    if (x.value())
      cout << x.value();
    else
      cout << "---";

    cout << '\n' << '\n';
  });
  cout << endl;
}

namespace {
using namespace options;

template <static_zstring str, typename type>
struct wrapper {
  static constexpr static_zstring name = str;
  constexpr wrapper() = default;
  constexpr wrapper(const type& x) : value{x} {}
  constexpr wrapper(type&& x) : value{move(x)} {}
  type value{};
};

}  // namespace

int main(int argc, char** argv) {
  tuple<wrapper<"first", int>, wrapper<"second", char>,
        wrapper<"third", string>>
      data{1, 'a', "help"};

  static_assert(is_named_tuple<decltype(data)>);

  cout << get_by_name<"first">(data).value << endl;
  cout << get_by_name<"second">(data).value << endl;
  cout << get_by_name<"third">(data).value << endl;

  get_by_name<"first">(data).value += 10;
  get_by_name<"third">(data).value += " me";

  cout << get_by_name<"first">(data).value << endl;
  cout << get_by_name<"second">(data).value << endl;
  cout << get_by_name<"third">(data).value << endl;

  try {
    options::parse<application::positioning>({argc, argv},
                                             application::options);
  } catch (exception& e) {
    cerr << e.what() << '\n';
    return -1;
  }

  print_options();
}

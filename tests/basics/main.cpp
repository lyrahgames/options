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

int main(int argc, char** argv) {
  using application::options;

  // lyrahgames::options::option_name x{"Hello"};
  // lyrahgames::options::option_name y{"Hello", 's'};
  // x.name_;
  // x.short_name_;
  // y.name_;
  // y.short_name_;

  // lyrahgames::options::basic_option<bool, "help", "..."> x{};
  // lyrahgames::options::basic_option<bool, {"version", 'v'}, "..."> y{};

  // static_assert(!lyrahgames::options::generic::has_short_name<decltype(x)>);
  // static_assert(lyrahgames::options::generic::has_short_name<decltype(y)>);
  // static_assert(decltype(y)::short_name() == 'v');
  // static_assert(decltype(y)::name() == "version");

  try {
    parse({argc, argv}, options);
  } catch (exception& e) {
    cerr << e.what() << '\n';
    return -1;
  }

  // value<"test">(options);

  if (value<"help">(options)) cout << "--help" << '\n';
  if (value<"version">(options)) cout << "--version" << '\n';
  if (value<"quiet">(options)) cout << "--quiet" << '\n';

  if (value<"input">(options))
    cout << "--input " << value<"input">(options) << '\n';
  if (value<"output">(options))
    cout << "--output " << value<"output">(options) << '\n';
  if (value<"type">(options))
    cout << "--type " << value<"type">(options) << '\n';

  if (value<"key">(options)) cout << "--key " << value<"key">(options) << '\n';
  // print_options();
  // cout << value<"key">(options) << '\n';
}

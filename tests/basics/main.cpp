#include <iomanip>
#include <iostream>
//
#include <lyrahgames/options/options.hpp>

using namespace std;
using namespace lyrahgames;

namespace application {
using namespace options;
option_list<  //
    flag<"help", "Print the help message.", 'h'>,
    flag<"version", "Print the help message.">,
    flag<"quiet", "Print the help message.", 'q'>,
    attachment<"input", "Provide program input.", 'i'>,
    attachment<"output", "Provide program output.", 'o'>,
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

#include <iomanip>
#include <iostream>
//
#include <lyrahgames/options/options.hpp>

using namespace std;

namespace application {
using namespace lyrahgames::options;
using options = option_list<  //
    flag<"help", "Print the help message.", 'h'>,
    flag<"version", "Print the help message.">,
    flag<"quiet", "Print the help message.", 'q'>,
    attachment<"input", "Provide program input.", 'i'>,
    attachment<"output", "Provide program output.", 'o'>,
    attachment<"type", "Provide program type.">,
    assignment<"key", "Provide a key.">>;
}  // namespace application
application::options options{};

void print_options() {
  for_each(options, [](auto& x) {
    cout << left << setw(20) << x.help() << '\n'
         << '\t' << setw(40) << x.description() << boolalpha << setw(8)
         << x.value << '\n'
         << '\n';
  });
  cout << endl;
}

int main(int argc, char** argv) {
  try {
    parse({argc, argv}, options);
  } catch (exception& e) {
    cerr << e.what() << '\n';
    return -1;
  }

  if (value<"help">(options)) cout << "--help" << '\n';
  if (value<"version">(options)) cout << "--version" << '\n';
  if (value<"quiet">(options)) cout << "--quiet" << '\n';

  if (*value<"input">(options))
    cout << "--input " << value<"input">(options) << '\n';
  if (*value<"output">(options))
    cout << "--output " << value<"output">(options) << '\n';
  if (*value<"type">(options))
    cout << "--type " << value<"type">(options) << '\n';

  if (*value<"key">(options)) cout << "--key " << value<"key">(options) << '\n';
  // print_options();
  // cout << value<"key">(options) << '\n';
}

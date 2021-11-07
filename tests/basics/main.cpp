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
    entry<"input", "Provide program input.", 'i'>,
    entry<"output", "Provide program output.", 'o'>,
    entry<"middle", "Provide program output.">,
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
  // print_options();

  try {
    parse({argc, argv}, options);
  } catch (lyrahgames::options::parse_error& e) {
    cerr << "ERROR: " << e.what() << '\n';
    // auto args = e.args();
    // while (!args.empty()) cerr << args.pop_front() << ' ';
    // cerr << endl;
  }

  print_options();

  cout << value<"key">(options) << '\n';
}

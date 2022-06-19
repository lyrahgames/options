#include <iomanip>
#include <iostream>
//
#include <lyrahgames/options/options.hpp>

using namespace std;
using namespace lyrahgames::options;

inline auto operator<<(ostream& os, const vector<czstring>& list) -> ostream& {
  if (list.empty()) return os;
  auto it = list.begin();
  os << *it++;
  for (; it != list.end(); ++it) os << ',' << *it;
  return os;
}

int main(int argc, char* argv[]) {
  option_list<  //
      flag<{"help", 'h'}, "Print the help message.">,
      flag<"version", "Print the library version.">,
      flag<{"quiet", 'q'}, "Set the program to be quiet.">,
      flag<"verbose", "Make program output verbose.">,
      appendable<{"input", 'i'}, "Provide an input file.">,
      attachment<{"output", 'o'}, "Set the output.">,
      assignment<"key", "Provide a key.">,
      fixed_attachment<{"fixed", 'f'}, "Provide a fixed value.">>
      options{};
  // using positioning = position_list<"output", {"input", undefined}>;

  try {
    parse({argc, argv}, options);
  } catch (exception& e) {
    cerr << e.what() << '\n';
    return -1;
  }

  for_each(options, [](auto& option) {
    cout << boolalpha;
    if (option) cout << option.name() << " = " << option.value() << endl;
  });
}

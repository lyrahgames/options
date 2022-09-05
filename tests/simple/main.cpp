#include <fstream>
#include <iomanip>
#include <iostream>
//
#include <lyrahgames/options/options.hpp>

// This application is able to print
// the content of a file to the command line.
// The application does exist at all times.
// We use a namespace singleton to make this clear.
// Also, a class could be used.
namespace application {

// Inside the application, we want the standard stuff to be available.
using namespace std;
using namespace lyrahgames;

// Simple logging strategy for the application.
// For this example, it should also be available as namespace singleton.
namespace log {
void info(auto&& x) {
  cout << "INFO:  " << forward<decltype(x)>(x) << endl;
}
void error(auto&& x) {
  cerr << "ERROR: " << forward<decltype(x)>(x) << endl;
}
}  // namespace log

// Provide the list of program options and
// the variable which is able to store their values.
using namespace options;
option_list<  //
    flag<{"help", 'h'}, "Print the help message.">,
    flag<"version", "Print the library version.">,
    appendable<{"input", 'i'}, "Provide an input file.">,
    attachment<{"output", 'o'}, "Set the output.">>
    options{};
using positioning = position_list<"output", {"input", undefined}>;

static_assert(is_valid_positioning<positioning, decltype(options)>::value);

// Initialize the application by parsing its command-line arguments.
void init(int argc, char* argv[]) {
  try {
    parse<positioning>({argc, argv}, options);
  } catch (options::parser_error& e) {
    log::error(e.what());
    exit(-1);
  }
}

// Run the actual application by interpreting the provided values.
void run() {
  // Provide a custom help message.
  if (option<"help">(options)) {
    cout << "This program is a simple test for the lyrahgames' options\n"
            "library and outputs a given file on the command line.\n"
         << endl;
    for_each(options, [](auto& option) {
      cout << left << setw(25) << option.help() << option.description() << endl;
    });
    exit(0);
  }

  // Provide the library version.
  if (option<"version">(options)) {
    cout << "Version " LYRAHGAMES_OPTIONS_VERSION_STR << endl;
    exit(0);
  }

  // Check for given input file in other cases.
  if (!option<"input">(options)) {
    log::error("No input file provided.");
    exit(-1);
  }

  // Open file and output it on the command line.
  for (auto str : value<"input">(options)) {
    cout << "File: " << str << "\n" << endl;
    fstream file{str, ios::in};
    string line;
    while (getline(file, line)) cout << line << endl;
    cout << "\n\n" << endl;
  }
}

}  // namespace application

int main(int argc, char* argv[]) {
  application::init(argc, argv);
  application::run();
}

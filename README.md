<h1 align="center">
    Lyrahgames' Program Options Package
</h1>

<p align="center">
    C++ Header-Only Library Implementing Simple Command-Line Arguments Parsing for Applications
</p>

## Development Status

<p align="center">
    <img src="https://img.shields.io/github/languages/top/lyrahgames/options.svg?style=for-the-badge">
    <img src="https://img.shields.io/github/languages/code-size/lyrahgames/options.svg?style=for-the-badge">
    <img src="https://img.shields.io/github/repo-size/lyrahgames/options.svg?style=for-the-badge">
    <a href="COPYING.md">
        <img src="https://img.shields.io/github/license/lyrahgames/options.svg?style=for-the-badge&color=blue">
    </a>
</p>

<b>
<table align="center">
    <tr>
        <td>
            master
        </td>
        <td>
            <a href="https://github.com/lyrahgames/options">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/options/master.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <!-- <td>
            <a href="https://circleci.com/gh/lyrahgames/options/tree/master"><img src="https://circleci.com/gh/lyrahgames/options/tree/master.svg?style=svg"></a>
        </td> -->
        <!-- <td>
            <a href="https://codecov.io/gh/lyrahgames/options">
              <img src="https://codecov.io/gh/lyrahgames/options/branch/master/graph/badge.svg" />
            </a>
        </td> -->
        <td>
            <a href="https://ci.stage.build2.org/?builds=lyrahgames-options&pv=&tc=*&cf=&mn=&tg=&rs=*">
                <img src="https://img.shields.io/badge/b|2 ci.stage.build2.org-Click here!-blue">
            </a>
        </td>
    </tr>
    <!-- <tr>
        <td>
            develop
        </td>
        <td>
            <a href="https://github.com/lyrahgames/options/tree/develop">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/options/develop.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/options/tree/develop"><img src="https://circleci.com/gh/lyrahgames/options/tree/develop.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/options">
              <img src="https://codecov.io/gh/lyrahgames/options/branch/develop/graph/badge.svg" />
            </a>
        </td>
    </tr> -->
    <tr>
        <td>
        </td>
    </tr>
    <tr>
        <td>
            Current
        </td>
        <td>
            <a href="https://github.com/lyrahgames/options">
                <img src="https://img.shields.io/github/commit-activity/y/lyrahgames/options.svg?logo=github&logoColor=white">
            </a>
        </td>
        <!-- <td>
            <img src="https://img.shields.io/github/release/lyrahgames/options.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/release-pre/lyrahgames/options.svg?label=pre-release&logo=github&logoColor=white">
        </td> -->
        <td>
            <img src="https://img.shields.io/github/tag/lyrahgames/options.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag-date/lyrahgames/options.svg?label=latest%20tag&logo=github&logoColor=white">
        </td>
        <!-- <td>
            <a href="https://queue.cppget.org/options">
                <img src="https://img.shields.io/website/https/queue.cppget.org/options.svg?down_message=empty&down_color=blue&label=b|2%20queue.cppget.org&up_color=orange&up_message=running">
            </a>
        </td> -->
    </tr>
</table>
</b>

## Requirements
<b>
<table>
    <tr>
        <td>Language Standard:</td>
        <td>C++20</td>
    </tr>
    <tr>
        <td>Compiler:</td>
        <td>
            GCC | Clang
        </td>
    </tr>
    <tr>
        <td>Build System:</td>
        <td>
            <a href="https://build2.org/">build2</a>
        </td>
    </tr>
    <tr>
        <td>Operating System:</td>
        <td>
            Linux
        </td>
    </tr>
    <tr>
        <td>Dependencies:</td>
        <td>
            <a href="http://github.com/lyrahgames/xstd">
                lyrahgames-xstd
            </a>
        </td>
    </tr>
</table>
</b>

## Getting Started

```c++
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
    flag<"help", "Print the help message.", 'h'>,
    flag<"version", "Print the library version.">,
    attachment<"input", "Provide an input file.", 'i'>>
    options{};

// Initialize the application by parsing its command-line arguments.
void init(int argc, char* argv[]) {
  try {
    parse({argc, argv}, options);
  } catch (options::parser_error& e) {
    log::error(e.what());
    exit(-1);
  }
}

// Run the actual application by interpreting the provided values.
void run() {
  // Provide a custom help message.
  if (value<"help">(options)) {
    cout << "This program is a simple test for the lyrahgames' options\n"
            "library and outputs a given file on the command line.\n"
         << endl;
    for_each(options, [](auto& option) {
      cout << left << setw(25) << option.help() << option.description() << endl;
    });
    exit(0);
  }

  // Provide the library version.
  if (value<"version">(options)) {
    cout << "Version " LYRAHGAMES_OPTIONS_VERSION_STR << endl;
    exit(0);
  }

  // Check for given input file in other cases.
  if (!value<"input">(options)) {
    log::error("No input file provided.");
    exit(-1);
  }

  // Open file and output it on the command line.
  fstream file{value<"input">(options), ios::in};
  string line;
  while (getline(file, line)) cout << line << endl;
}

}  // namespace application

int main(int argc, char* argv[]) {
  application::init(argc, argv);
  application::run();
}
```

Example Output:
```
$ ./program --help
This program is a simple test for the lyrahgames' options
library and outputs a given file on the command line.

-h, --help               Print the help message.
--version                Print the library version.
-i, --input <value>      Provide an input file.
```

## Usage with build2
Add this repository to the `repositories.manifest` file of your build2 package.

    :
    role: prerequisite
    location: https://github.com/lyrahgames/options.git

Add the following entry to the `manifest` file with a possible version dependency.

    depends: lyrahgames-options

Add these entries to your `buildfile`.

    import libs = lyrahgames-options%lib{lyrahgames-options}
    exe{your-executable}: {hxx cxx}{**} $libs


## Installation
The standard installation process will only install the header-only library with some additional description, library, and package files.

    bpkg -d build2-packages cc \
      config.install.root=/usr/local \
      config.install.sudo=sudo

Get the latest package release and build it.

    bpkg build https://github.com/lyrahgames/options.git

Install the built package.

    bpkg install lyrahgames-options

For uninstalling, do the following.

    bpkg uninstall lyrahgames-options

If your package uses an explicit `depends: lyrahgames-options` make sure to initialize this dependency as a system dependency when creating a new configuration.

    bdep init -C @build cc config.cxx=g++ "config.cxx.coptions=-O3" -- "?sys:lyrahgames-options/*"

## Alternative Usage
To use other build systems or manual compilation, you only have to add the `lyrahgames/options/` directory to your project and include it in the compilation process.

## Background
Assume, we would like to create a program which is able to accept the three flags `--help`, `--version`, and `--input` on the command line.
Hereby, `--help` and `--version` shall not provide any further arguments but only indicate that the help message or the version string should be printed on the screen.
Instead, the `--input` flag requires a subsequent argument to provide a file path to be read.
A simple way to manually implement these flags for the program is given in the following code snippet.

```c++
#include <iostream>
#include <string>

using namespace std;

// State Variables for Program Options
bool help_flag = false;
bool version_flag = false;
const char* input_flag = nullptr;

// Custom Implementation
void print_help() { ... }
void print_version() { ... }
void print_file(const char* path) { ... }

int main(int argc, char* argv[]) {
    // Parse argv into the program option state.
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "--help"))
            help_flag = true;
        else if (strcmp(argv[i], "--version"))
            version_flag = true;
        else if (strcmp(argv[i], "--input")){
            ++i;
            if (i >= argc) {
                cerr << "ERROR: No given value for option '--input'." << endl;
                exit(-1);
            }
            input_flag = argv[i];
        } else {
            cerr << "ERROR: Unknown option " << argv[i] << endl;
            exit(-1);
        }
        ++i;
    }

    // After parsing, run the code by using the program option state.
    if (help_flag) {
        print_help();
        exit(0);
    }
    if (version_flag) {
        print_version();
        exit(0);
    }
    if (!input_flag) {
        cerr << "ERROR: No input file provided." << endl;
        exit(-1);
    }
    print_file(input_flag);
}
```

It is obvious that this design technique will become error-prone quite fast.
Additionally, it looks really generic and it should be possible to automatically generate such code for a given list of flags.

- Why no callbacks for flags? -> live execution is bad, you want to keep the state of parsed flags, does not give you any performance, code more difficult to reason about
- Simple Parsing L(k): Right now, it is string comparison. This seems to be fast enough. Otherwise, we could use a radix tree for name checking.
- Efficient Storage
- Easy Naming and access
- extendable
- only bool and czstring useful: do not parse everything
- Compile-time, static
- flags, attachments, assignments
- Currently, no free-form/positional arguments

Assumptions
- not too many arguments (compiler flags are quite strange)
- completely static and nothing is dynamic
- simple parsing is fast enough

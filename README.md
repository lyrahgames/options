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
#include <iostream>
#include <iomanip>
//
#include <lyrahgames/options/options.hpp>

using namespace std;

namespace application{
  using namespace lyrahgames::options;
  using options = option_list<
    flag<"help", "Print the help message.", 'h'>,
    flag<"version", "Print the program version.">,
    attachment<"input", "Provide an input file path.", 'i'>,
    assignment<"key", "Assign a key.">>;
}
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

int main(int argc, char* argv[]) {
  parse({argc, argv}, options);
  print_options();
  cout << "key = " << value<"key">(options) << '\n';
}
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
- Simple Parsing L(k): Right now, it is string comparison. This seems to be fast enough. Otherwise, we could use a radix tree for name checking.
- Efficient Storage
- Easy Naming and access
- extendable
- only bool and czstring useful: do not parse everything
- Compile-time, static
- flags, attachments, assignments

[![GitHub version](https://badge.fury.io/gh/ainfosec%2Fci_helloworld.svg)](https://badge.fury.io/gh/ainfosec%2Fci_helloworld)
[![Build Status](https://travis-ci.org/ainfosec/ci_helloworld.svg?branch=master)](https://travis-ci.org/ainfosec/ci_helloworld)
[![Build status](https://ci.appveyor.com/api/projects/status/b8ceqhe5n1884ek5/branch/master?svg=true)](https://ci.appveyor.com/project/rianquinn/ci-helloworld/branch/master)
[![codecov](https://codecov.io/gh/ainfosec/ci_helloworld/branch/master/graph/badge.svg)](https://codecov.io/gh/ainfosec/ci_helloworld)
[![Coverage Status](https://coveralls.io/repos/github/ainfosec/ci_helloworld/badge.svg?branch=master)](https://coveralls.io/github/ainfosec/ci_helloworld?branch=master)
<a href="https://scan.coverity.com/projects/ainfosec-ci_helloworld">
  <img alt="Coverity Scan Build Status"
       src="https://img.shields.io/coverity/scan/12883.svg"/>
</a>
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/8e0fdf1c93ab47f1ab3bbe5777f747ba)](https://www.codacy.com/app/rianquinn/ci_helloworld?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ainfosec/ci_helloworld&amp;utm_campaign=Badge_Grade)
[![Join the chat at https://gitter.im/ci_helloworld/Lobby](https://badges.gitter.im/ci_helloworld/Lobby.svg)](https://gitter.im/ci_helloworld/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## Description

This repo provides a simple example for how to setup various CI services as
well as integrating analysis tools into these services. These tools should be
used as part of a comprehensive Software Development Process (SDP) and can also
be used as a starting template for any C or C++ application. The following CI
tools are used, providing testing support for Windows, Cygwin, Linux and macOS

- [Travis CI](https://travis-ci.org/)
- [AppVeyor](https://www.appveyor.com/)

The following checks are performed:

- [Doxygen](http://www.doxygen.org)
- [Git Check](https://git-scm.com/docs/git-diff)
- [Astyle](http://astyle.sourceforge.net/)
- [Clang Tidy](http://clang.llvm.org/extra/clang-tidy/)
- [CppCheck](http://cppcheck.sourceforge.net/)
- [Codacy](https://www.codacy.com)
- [Coverity Scan](https://scan.coverity.com/)
- [Codecov](https://codecov.io/)
- [Coveralls](https://coveralls.io/)
- [Google Sanitizers](https://github.com/google/sanitizers)
- [Valgrind](http://valgrind.org/)
- [GCC Compiler Tests](https://gcc.gnu.org/)
- [Clang Compiler Tests](https://clang.llvm.org/)
- [XCode Compiler Tests](https://developer.apple.com/xcode/)
- [Visual Studio Compiler Tests](https://www.visualstudio.com/)

The following real world projects use a variety of these techniques as part
of their SDP:

- [Bareflank Hypervisor](https://github.com/Bareflank/hypervisor)
- [JSON](https://github.com/nlohmann/json)
- [Neovim](https://github.com/neovim/neovim)
- [GSL](https://github.com/Microsoft/GSL)

## Dependencies

Although this repo can be made to run on most systems, the following are the
supported platforms and their dependencies:

#### Ubuntu 16.10 (or Higher):
```
sudo apt-get install git build-essential cmake
```

#### Windows (Cygwin):
```
setup-x86_64.exe -q -P git,make,gcc-core,gcc-g++,cmake
```

#### Windows (Visual Studio):

Install the following packages:
- [Visual Studio SDK 10](https://go.microsoft.com/fwlink/?linkid=838916)
- [Visual Studio 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15#)
  - Check "Desktop development with C++"
  - Check "C++ CLI / Support"
  - Check "Standard Library Modules"
- [CMake v3.6+](https://cmake.org/download/)
- [Git for Windows](https://github.com/git-for-windows/git/releases)

#### macOS:
- [XCode 7.3+](https://developer.apple.com/xcode/)
- [CMake v3.6+](https://cmake.org/download/)

## Compilation / Testing / Installation

To compile and install this example, use the following instructions:

#### GCC / Clang
```
git clone https://github.com/ainfosec/ci_helloworld.git

mkdir ci_helloworld/build
cd ci_helloworld/build

cmake ..

make
make test
```

#### Visual Studio 2017 (NMake)
```
git clone https://github.com/ainfosec/ci_helloworld.git

mkdir ci_helloworld/build
cd ci_helloworld/build

cmake -G "NMake Makefiles" ..

nmake
nmake test
```

#### Visual Studio 2017 (MSBuild)
```
git clone https://github.com/ainfosec/ci_helloworld.git

mkdir ci_helloworld/build
cd ci_helloworld/build

cmake -G "Visual Studio 15 2017 Win64" ..

msbuild ci_helloworld.sln
ctest
```

#### XCode 7.3+
```
git clone https://github.com/ainfosec/ci_helloworld.git

mkdir ci_helloworld/build
cd ci_helloworld/build

cmake ..

make
make test
```

## Analysis Tools

The following provides a description of all of the analysis tools that have
been integrated into the CI services used by this project including an
explanation of how it works.

### Doxygen

The CI is setup to check for missing documentation using doxygen. Unlike most
of the analysis tools used in this project, there is no make target for
doxygen, and instead it is run using doxygen manually with the following
script:

```yml
- doxygen .doxygen.txt
- |
  if [[ -s doxygen_warnings.txt ]]; then
    echo "You must fix doxygen before submitting a pull request"
    echo ""
    cat doxygen_warnings.txt
    exit -1
  fi
```

This script runs doxygen against the source code and any warnings are placed
into a file called `doxygen_warnings.txt`. If this file is empty, it means that
the doxygen analysis passed, and all of the code is documented based on the
settings in the `.doxygen.txt` configuration file. If this files is not
empty, the test fails, and prints the warnings generated by doxygen.

### Git Check

`git diff --check` provides a simple way to detect when whitespace errors
has been checked into the repo, as well as checking when end-of-file newlines
are either missing, or contain too many. More information about this check
can be found [here](https://git-scm.com/docs/git-diff). This check is extremely
useful for developers when PRs contain modifications unrelated to their specific
changes.

```yml
- |
  if [[ -n $(git diff --check HEAD^) ]]; then
    echo "You must remove whitespace before submitting a pull request"
    echo ""
    git diff --check HEAD^
    exit -1
  fi
```

This check simply runs `git diff --check`, which returns with an error if the
check fails. If this occurs, the diff is displayed for the user to see.

### Astyle

Source code formatting is a great way to keep a consistent look and feel with
the code. The problem with source formatting is, unless everyone is using it,
developers PRs will contain modifications unrelated to their specific
changes, or worse, attempting to fix source formatting periodically will
destroy your repo's git history each time you format the source. Therefore,
if source formatting is to be used, it should be checked on every code
diff to ensure the formatting is correct.

For this example, we use Astyle, but Clang Format will also work. To support
Astyle in a simple way, we provide a make target that allows the developer
to format their source code by simply running `make format`. To do this,
we must first get Astyle:

```cmake
list(APPEND ASTYLE_CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
)

ExternalProject_Add(
    astyle
    GIT_REPOSITORY      https://github.com/Bareflank/astyle.git
    GIT_TAG             v1.2
    GIT_SHALLOW         1
    CMAKE_ARGS          ${ASTYLE_CMAKE_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/astyle/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/astyle/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/astyle/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/astyle/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/astyle/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/astyle/build
)
```

This cmake logic uses ExternalProject_Add to automatically download Astyle,
compile it for your platform, and install it into your build directory so that
it can be used by our custom make target. Note that we use our own patched
version of Astyle that changes the build system from Astyle's custom set of
Makefiles to a CMake build system for simplicity.

```cmake
list(APPEND ASTYLE_ARGS
    --style=1tbs
    --lineend=linux
    --suffix=none
    --pad-oper
    --unpad-paren
    --break-closing-brackets
    --align-pointer=name
    --align-reference=name
    --indent-preproc-define
    --indent-switches
    --indent-col1-comments
    --keep-one-line-statements
    --keep-one-line-blocks
    --pad-header
    --convert-tabs
    --min-conditional-indent=0
    --indent=spaces=4
    --close-templates
    --add-brackets
    --break-after-logical
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/test/*.cpp
)

if(NOT WIN32 STREQUAL "1")
    add_custom_target(
        format
        COMMAND ${CMAKE_SOURCE_DIR}/bin/astyle ${ASTYLE_ARGS}
        COMMENT "running astyle"
    )
else()
    add_custom_target(
        format
        COMMAND ${CMAKE_SOURCE_DIR}/bin/astyle.exe ${ASTYLE_ARGS}
        COMMENT "running astyle"
    )
endif()
```

To create our custom astyle make target, we use the above CMake code. This
points CMake to the resulting astyle binary depending on the platform, and
provides astyle with the formatting options and source files specific to this
project.

```yml
- cmake -DENABLE_ASTYLE=ON -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- make format
- |
  if [[ -n $(git diff) ]]; then
    echo "You must run make format before submitting a pull request"
    echo ""
    git diff
    exit -1
  fi
```

Finally, to verify on each PR that a code change adheres to our Astyle
configuration, we add the above code to our Travis CI script. This creates
our `make format` target and executes it to format the code. If `make format`
formats the code, a diff will be created which `git diff` can be used to detect.
If no diff is created, it means all of the source adheres to our Astyle
configuration, and the test passes.

### Clang Tidy

Clang Tidy provides static analysis. Support for this tool starts by adding the
following to the CMakeLists.txt:

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

This tells CMake to record all of the compilation instructions used to compile
your project including flags and definitions. Clang Tidy will use this
information to statically analyze your project the same way it was compiled.
The advantage to this approach is a significant improvement in accuracy. The
main disadvantage to this approach is Clang Tidy is not good at statically
analyzing files that do not show up in this compilation database (such as
header files). For this reason, if you want to analyze a header, it has to
be included by a source file that is included in the compilation database.

```cmake
list(APPEND RUN_CLANG_TIDY_BIN_ARGS
    -clang-tidy-binary ${CLANG_TIDY_BIN}
    -header-filter=.*
    -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
)

add_custom_target(
    tidy
    COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
    COMMENT "running clang tidy"
)
```

Finally, Clang Tidy is given its own make target to simplify its use. Here
we tell the `run-clang-tidy-4.0.py` script which clang tidy binary to use,
as well as which checks to perform, and what header files to include, which
is all of them. We turn off the -cert-err58-cpp test because it triggers
code from catch.hpp, and -misc-noexcept-move-constructor because it is still
buggy in 4.0. Note that we choose a specific version of Clang Tidy which is
important because each new version of Clang Tidy fixes bugs and adds new
checks, resulting in different results depending on which version you use.

```yml
- cmake -DENABLE_CLANG_TIDY=ON -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- make tidy > output.txt
- |
  if [[ -n $(grep "warning: " output.txt) ]] || [[ -n $(grep "error: " output.txt) ]]; then
      echo "You must pass the clang tidy checks before submitting a pull request"
      echo ""
      grep --color -E '^|warning: |error: ' output.txt
      exit -1;
  else
      echo -e "\033[1;32m\xE2\x9C\x93 passed:\033[0m $1";
  fi
```

From Travis CI, we enable Clang Tidy, and dump its output to a file. If this
file contains "warning" or "error" we fail the test, and output the issues
reported by Clang Tidy to the user to be fixed. This ensures that every
PR has been statically checked.

### CppCheck

CppCheck is another static analysis tool.

```cmake
list(APPEND CPPCHECK_CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
)

ExternalProject_Add(
    cppcheck
    GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
    GIT_TAG             1.79
    GIT_SHALLOW         1
    CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
)
```

The version of CppCheck provided by Ubuntu 14.04 is old, and does not support
C++11 well, so we grab a specific version of CppCheck from GitHub, allowing
all users of the project to use the same version.

```cmake
list(APPEND CPPCHECK_ARGS
    --enable=warning,style,performance,portability,unusedFunction
    --std=c++11
    --verbose
    --error-exitcode=1
    --language=c++
    -DMAIN=main
    -I ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/test/*.cpp
)

add_custom_target(
    check
    COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
    COMMENT "running cppcheck"
)
```

We then add a custom target for our newly built CppCheck application, telling
CppCheck to enable all of its checks (minus pedantic warnings) and to check
all of our source files. Note that CppCheck needs to know that MAIN=main,
otherwise it will think that the main function is not executed, and we need
to tell CppCheck to error with a non-0 error code so that Travis CI reports
a failed test if any of the checks fail.

```yml
- cmake -DENABLE_CPPCHECK=ON -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- make check
```

Running the Travis CI test is as simply as turning on CppCheck, and running
the custom make target.

### Coverity Scan

Coverity Scan is another static analysis tool that is very good at finding hard
to find structural issues with your code. If you have access to Coverity Scan,
it is well worth adding to your SDP.

```yml
- os: linux
  env:
    - TEST="Coverity Scan"
  addons:
    apt:
      sources:
        - ubuntu-toolchain-r-test
      packages:
        - gcc-6
        - g++-6
    coverity_scan:
      project:
        name: "ainfosec/ci_helloworld"
        description: "A simple example of how to setup a complete CI environment for C and C++"
      notification_email: rianquinn@gmail.com
      build_command_prepend: "cmake -DCMAKE_CXX_COMPILER=g++-6 .."
      build_command: "make"
      branch_pattern: master
  script:
    - echo -n | openssl s_client -connect scan.coverity.com:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' | sudo tee -a /etc/ssl/certs/ca-
```

Coverity Scan is also very simple to setup. The above Travis CI test is
a cut/paste from their website after you register your project. All we have to
do is compile the source which tells Coverity Scan how the source code is
compiled. From there, their website will provide a means to exclude directories
and see issues with the code. In our example, we do a scan on every change to
master as the number of changes to master is small, but on large projects with
a lot of merges per day, Coverity Scan suggests using a specific coverity_scan
branch for scans. If this is done, a nightly scan should be setup by grabbing
the master branch, and pushing it to the coverity_scan branch each night.
This way, issues with Coverity Scan can be identified quickly.

### Codecov

Codecov is a powerful, yet simple to setup coverage tool.

```cmake
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()
```

To setup coverage, we must enable GCOV support in our compiler (assumes
GCC or Clang). Once this support is enabled, running `make test` will generate
coverage stats that Codecov can analyze to give you a report of what code has
or has not been unit tested.

```yml
- cmake -DENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- make test
- cd ..
- bash <(curl -s https://codecov.io/bash)
```

The Travis CI test is as simple as compiling and running the unit tests,
and then running Codecov's bash script. Once this is done, the results can
be see on Codecov's website.

### Coveralls

Coveralls is another coverage tool.

```cmake
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()
```

Like Codecov, GCOV has to be enabled.

```yml
- pip install --user git+git://github.com/eddyxu/cpp-coveralls.git
- cmake -DENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- make test
- cd ..
- |
  coveralls --build-root build --gcov-options '\-lp' \
    -e build/external \
    -e build/include \
    -e build/CMakeFiles/3.8.0 \
    -e build/CMakeFiles/feature_tests.c \
    -e build/CMakeFiles/feature_tests.cxx
```

Unlike Codecov, Coveralls is a lot harder to setup. Codecov keeps track of
which files are in your git repository and only generates reports for files
in the repo, while Coveralls will generate coverage reports for all files it
sees, including generated files by CMake. Coveralls also does not have a
simple bash script to report coverage data to their server, but instead requires
the installation of an external C++ specific tool for collecting GCOV data.
For these reasons, we have to install cpp-coveralls, and then tell it to
exclude specific files / directories that are being collected that should not
be.

### Google Sanitizers

The Google Sanitizers are a dynamic analysis tool that is included in GCC
and Clang/LLVM.

```cmake
if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif()

if(ENABLE_USAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

if(ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()
```

Each sanitizer has to be run in isolation, and thus we have one test per
sanitizer group. The flags for each set can be found on Google's GitHub page
as well as Clang's usage documentation.

```cmake
- cmake -DENABLE_ASAN=ON -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- make test
```

For each test, we turn on the specific check, and the unit tests, and if
a check fails, the unit test will exit with a non-0 exit code, causing
Travis CI to fail the test. It should be noted that each new version of
GCC and Clang comes with better support, and thus, like some of the other
tools, you should stick to a specific version.

### Valgrind

Valgrind is another dynamic analysis tool that provides leak detection.

```cmake
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --track-fds=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1")
```

The easiest way to execute Valgrind is to use CMake's built-in support as it
will handle error logic for you. For this reason, we need to tell CMake what
flags to give Valgrind. In this case we enable all of its checks and tell
Valgrind to exit with a non-0 exit code so that if a check fails, Travis CI
will fail the test.

```yml
- cmake -DCMAKE_CXX_COMPILER="g++-6" ..
- make
- ctest -T memcheck
```

To run the test, all we need to do is compile the code, and run the unit tests
using ctest, enabling the memcheck mode.

## License

This project is licensed under the MIT License.

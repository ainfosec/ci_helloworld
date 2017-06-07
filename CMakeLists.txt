#
# CI Hello World
#
# Copyright (C) 2017 Assured Information Security, Inc.
# Author: Rian Quinn        <quinnr@ainfosec.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.6)
project(ci_helloworld C CXX)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include_directories(include)
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/include)

# ------------------------------------------------------------------------------
# Catch
# ------------------------------------------------------------------------------

list(APPEND CATCH_CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
)

ExternalProject_Add(
    catch
    GIT_REPOSITORY      https://github.com/Bareflank/catch.git
    GIT_TAG             v1.2
    GIT_SHALLOW         1
    CMAKE_ARGS          ${CATCH_CMAKE_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/catch/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/catch/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/catch/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/catch/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/catch/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/catch/build
)

# ------------------------------------------------------------------------------
# Hippomocks
# ------------------------------------------------------------------------------

list(APPEND HIPPOMOCKS_CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
)

ExternalProject_Add(
    hippomocks
    GIT_REPOSITORY      https://github.com/Bareflank/hippomocks.git
    GIT_TAG             v1.2
    GIT_SHALLOW         1
    CMAKE_ARGS          ${HIPPOMOCKS_CMAKE_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/hippomocks/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/hippomocks/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/hippomocks/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/hippomocks/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/hippomocks/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/hippomocks/build
)

# ------------------------------------------------------------------------------
# GSL
# ------------------------------------------------------------------------------

list(APPEND GSL_CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
)

ExternalProject_Add(
    gsl
    GIT_REPOSITORY      https://github.com/Bareflank/GSL.git
    GIT_TAG             v1.2
    GIT_SHALLOW         1
    CMAKE_ARGS          ${GSL_CMAKE_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/gsl/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/gsl/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/gsl/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/gsl/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/gsl/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/gsl/build
)

# ------------------------------------------------------------------------------
# Astyle
# ------------------------------------------------------------------------------

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

# ------------------------------------------------------------------------------
# Main Executable
# ------------------------------------------------------------------------------

add_executable(ci_helloworld src/main.cpp src/producer.cpp)
add_dependencies(ci_helloworld gsl)
target_compile_definitions(ci_helloworld PRIVATE VIRTUAL= MAIN=main)

# ------------------------------------------------------------------------------
# Unit Testing
# ------------------------------------------------------------------------------

include(CTest)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fuse-ld=gold -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak")
endif()

if(ENABLE_USAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold -fsanitize=undefined")
endif()

add_library(test_catch STATIC test/test.cpp)
add_dependencies(test_catch catch)

add_library(business_logic STATIC src/main.cpp src/producer.cpp)
add_dependencies(business_logic gsl)
target_compile_definitions(business_logic PRIVATE VIRTUAL=virtual MAIN=unittest_main)

macro(do_test str)
    add_executable(test_${str} test/test_${str}.cpp)
    add_test(test_${str} test_${str})
    target_link_libraries(test_${str} test_catch)
    target_link_libraries(test_${str} business_logic)
    add_dependencies(test_${str} gsl)
    add_dependencies(test_${str} catch)
    add_dependencies(test_${str} hippomocks)
    target_compile_definitions(test_${str} PRIVATE VIRTUAL=virtual)
endmacro(do_test)

do_test(main)
do_test(producer)
do_test(consumer)

# ------------------------------------------------------------------------------
# Valgrind
# ------------------------------------------------------------------------------

find_program(VALGRIND "valgrind")

if(VALGRIND STREQUAL "VALGRIND-NOTFOUND")
    message(FATAL_ERROR "unable to locate valgrind")
endif()

add_custom_target(
    valgrind
    COMMAND ${VALGRIND} --error-exitcode=1 --leak-check=yes ctest
)

# ------------------------------------------------------------------------------
# Astyle
# ------------------------------------------------------------------------------

list(APPEND ASTYLE_ARGS
    "--style=1tbs"
    "--lineend=linux"
    "--suffix=none"
    "--pad-oper"
    "--unpad-paren"
    "--break-closing-brackets"
    "--align-pointer=name"
    "--align-reference=name"
    "--indent-preproc-define"
    "--indent-switches"
    "--indent-col1-comments"
    "--keep-one-line-statements"
    "--keep-one-line-blocks"
    "--pad-header"
    "--convert-tabs"
    "--min-conditional-indent=0"
    "--indent=spaces=4"
    "--close-templates"
    "--add-brackets"
    "--break-after-logical"
)

add_custom_target(
    format
    COMMAND ${CMAKE_BINARY_DIR}/bin/astyle ${ASTYLE_ARGS} ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/test/*.cpp
    COMMENT "running astyle"
)

# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

find_program(CLANG_TIDY clang-tidy-4.0)
find_program(RUN_CLANG_TIDY run-clang-tidy-4.0.py)

if(CLANG_TIDY STREQUAL "CLANG_TIDY-NOTFOUND")
    message(FATAL_ERROR "unable to locate clang-tidy-4.0")
endif()

if(RUN_CLANG_TIDY STREQUAL "RUN_CLANG_TIDY-NOTFOUND")
    message(FATAL_ERROR "unable to locate run-clang-tidy-4.0.py")
endif()

list(APPEND RUN_CLANG_TIDY_ARGS
    -clang-tidy-binary ${CLANG_TIDY}
    -header-filter=.*
    -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
)

add_custom_target(
    tidy
    COMMAND ${RUN_CLANG_TIDY} ${RUN_CLANG_TIDY_ARGS}
    COMMENT "running clang tidy"
)

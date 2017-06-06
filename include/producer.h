//
// CI Hello World
//
// Copyright (C) 2017 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PRODUCER_H
#define PRODUCER_H

#include <iostream>

/// Producer
///
/// A simple example of a producer that is capable of being mocked while
/// unit testing occurs.
///
class producer
{
public:

    /// Default Constructor
    ///
    producer() = default;

    /// Default Destructor
    ///
    VIRTUAL ~producer() = default;

    /// Print Message
    ///
    /// Prints hello world to the console. The function is marked "VIRTUAL"
    /// instead of "virtual" because the this class is not intended to be
    /// subclassed, but "virtual" is required for unit testing. Therefore,
    /// this class is compiled twice:
    /// - the main application defines VIRTUAL=
    /// - the unit test recompiles this source defining VIRTUAL=virtual
    ///
    VIRTUAL void print_msg()
    { std::cout << "hello world\n"; }

public:

    // We define the copy and move constructors / operators because Clang Tidy
    // (legitimately) complains about the definition of a destructor without
    // defining the copy and move semantics for this class. In general a class
    // should always be marked non-copyable unless such functionality is
    // specifically desired.

    producer(producer &&) noexcept = default;               ///< Default move construction
    producer &operator=(producer &&) noexcept = default;    ///< Default move operator

    producer(const producer &) = delete;                    ///< Deleted copy construction
    producer &operator=(const producer &) = delete;         ///< Deleted copy operator
};

#endif

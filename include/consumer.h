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

#ifndef CONSUMER_H
#define CONSUMER_H

#include <gsl/gsl>
#include <producer.h>

/// Consumer
///
/// A simple example of a consumer that is capable of accepting the real
/// producer, or a mocked version for unit testing.
///
class consumer
{
public:

    /// Default Constructor
    ///
    /// @param p the producer that this consumer will use.
    ///
    explicit consumer(gsl::not_null<producer *> p)
    { p->print_msg(); }

    /// Default Destructor
    ///
    ~consumer() = default;

public:

    // We define the copy and move constructors / operators because Clang Tidy
    // (legitimately) complains about the definition of a destructor without
    // defining the copy and move semantics for this class. In general a class
    // should always be marked non-copyable unless such functionality is
    // specifically desired.

    consumer(consumer &&) noexcept = default;               ///< Default move construction
    consumer &operator=(consumer &&) noexcept = default;    ///< Default move operator

    consumer(const consumer &) = delete;                    ///< Deleted copy construction
    consumer &operator=(const consumer &) = delete;         ///< Deleted copy operator
};

#endif

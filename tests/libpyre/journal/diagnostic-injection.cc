// -*- coding: utf-8 -*-
//
// michael a.g. aïvázis
// california institute of technology
// (c) 1998-2011 all rights reserved
//


// for the build system
#include <portinfo>

// packages
#include <assert.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

// access to the low level diagnostic header file
#include <pyre/journal/Chronicler.h>
#include <pyre/journal/Diagnostic.h>
#include <pyre/journal/macros.h>

#include <pyre/journal/Locator.h>
#include <pyre/journal/Selector.h>
#include <pyre/journal/manipulators.h>


// a simple channel class
class Debug : public pyre::journal::Diagnostic<Debug> {
    // types
public:
    typedef std::string string_t;
    // meta methods
public:
    Debug(string_t name) : Diagnostic<Debug>("debug", name) {}
};


// main program
int main() {

    // instantiate
    Debug d("pyre.journal.test");
    // inject
    d << pyre::journal::Selector("key", "value");
    d << pyre::journal::Locator(__HERE__);
    d << "Hello world!";
    d << pyre::journal::newline;
    d << pyre::journal::endl;;

    // all done
    return 0;
}

// end of file

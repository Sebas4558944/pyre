// -*- c++ -*-
//
// michael a.g. aïvázis <michael.aivazis@para-sim.com>
// (c) 1998-2023 all rights reserved


// get the journal
#include <pyre/journal.h>
// support
#include <cassert>


// exercise the channel manipulators
int main() {
    // make a debug channel
    pyre::journal::debug_t channel("tests.journal.debug");

    // activate it
    channel.activate();
    // but send the output to the trash
    channel.device<pyre::journal::trash_t>();

    // inject something into the channel
    channel
        // location
        << pyre::journal::at(__HERE__)
        // some metadata
        << pyre::journal::note("time", "now")
        // a message with a newline
        << "debug channel:" << pyre::journal::newline
        // another message and a flush
        << "    hello world!" << pyre::journal::endl;

    // all done
    return 0;
}


// end of file

// -*- c++ -*-
//
// michael a.g. aïvázis <michael.aivazis@para-sim.com>
// (c) 1998-2022 all rights reserved


// get the journal
#include <pyre/journal.h>
// support
#include <cassert>


// verify that empty injections in info channels happens correctly
int main() {
    // make an info channel
    pyre::journal::info_t channel("tests.journal.info");
    // send the output to the trash
    channel.device<pyre::journal::trash_t>();

    // inject nothing
    channel << pyre::journal::endl;

    // all done
    return 0;
}


// end of file

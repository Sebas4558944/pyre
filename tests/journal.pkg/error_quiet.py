#! /usr/bin/env python3
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2022 all rights reserved


def test():
    """
    Suppress all error output
    """
    # get the channel
    from journal.Error import Error as error

    # suppress the output
    error.quiet()

    # make an error channel
    channel = error(name="tests.journal.error")
    # add some metadata
    channel.notes["time"] = "now"

    # carefully
    try:
        # inject
        channel.line("error channel:")
        channel.log("    hello world!")
        # shouldn't get here
        assert False, "unreachable"
    # if the correct exception was raised
    except channel.ApplicationError as error:
        # all  good
        pass

    # all done
    return


# main
if __name__ == "__main__":
    # prohibit the journal bindings
    journal_no_libjournal = True
    # run the test
    test()


# end of file

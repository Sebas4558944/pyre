#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2012 all rights reserved
#


"""
Verify that we can process an empty pml file
"""


def test():
    # package access
    import pyre.config
    # get the codec manager
    m = pyre.config.newCodecManager()
    # ask for a pml codec
    reader = m.newCodec(encoding="pml")
    # the configuration file
    uri = "sample-empty.pml"
    # open a stream
    sample = open(uri)
    # read the contents
    events = reader.decode(uri=uri, source=sample, locator=None)
    # check that we got a real instance back
    assert events == []

    return m, reader, events


# main
if __name__ == "__main__":
    # skip pyre initialization since we don't rely on the executive
    pyre_noboot = True
    # do...
    test()


# end of file 

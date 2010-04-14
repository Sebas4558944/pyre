#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


"""
Exercises the fileserver
"""


def test():
    import pyre.framework
    # build the executive
    executive = pyre.framework.executive()
    # verify the right parts were built
    assert executive.codecs is not None
    assert executive.fileserver is not None
    assert executive.configurator is not None
    assert executive.calculator is not None
    # load a configuration file
    executive.loadConfiguration("sample.pml")
    # check that all is as expected
    assert executive.calculator["pyre.home"] == "pyre.home()"
    assert executive.calculator["pyre.prefix"] == "pyre.prefix()"
    assert executive.calculator["pyre.user.name"] == "michael a.g. aïvázis"
    assert executive.calculator["pyre.user.email"] == "aivazis@caltech.edu"
    assert executive.calculator["pyre.user.affiliation"] == "california institute of technology"

    # all done
    return executive


# main
if __name__ == "__main__":
    test()


# end of file 

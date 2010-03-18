#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


"""
Create and dump a local filesystem
"""


def test():
    import pyre.filesystem

    home = pyre.filesystem.newLocalFilesystem(root="../../../..")
    home._dump(interactive=False) # change to True to see the dump

    return


# main
if __name__ == "__main__":
    import pyre.filesystem
    # adjust the package metaclasses
    from pyre.patterns.ExtentAware import ExtentAware
    pyre.filesystem._metaclass_Node = ExtentAware
    pyre.filesystem._metaclass_Filesystem = ExtentAware

    test()

    # check that the filesystem was destroyed
    from pyre.filesystem.Filesystem import Filesystem
    # print("Filesystem extent:", len(Filesystem._pyre_extent))
    assert len(Filesystem._pyre_extent) == 0

    # now check that the nodes were all destroyed
    from pyre.filesystem.Node import Node
    # print("Node extent:", len(Node._pyre_extent))
    assert len(Node._pyre_extent) == 0


# end of file 

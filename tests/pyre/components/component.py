#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


"""
Check that declarations of trivial components produce the expected layout
"""


def test():
    import pyre

    # declare
    class component(pyre.component):
        """a trivial component"""

    # check the basics
    assert component.__name__ == "component"
    assert component.__bases__ == (pyre.component,)

    # check the layout
    assert component.pyre_name == "component"
    assert component.pyre_state == "registered"
    assert component.pyre_inventory == {}
    assert component.pyre_namemap == {}
    assert component.pyre_traits == ()
    assert component.pyre_pedigree == (component, pyre.component)
    assert component.pyre_family == []
    assert component.pyre_implements == None

    # exercise the configurable interface
    assert tuple(component.pyre_getTraitDescriptors()) == ()
    assert component.pyre_isCompatible(component)

    # exercise the component interface
    assert component.pyre_getPackageName() == None
    assert tuple(component.pyre_getExtent()) == ()

    return component


# main
if __name__ == "__main__":
    test()


# end of file 

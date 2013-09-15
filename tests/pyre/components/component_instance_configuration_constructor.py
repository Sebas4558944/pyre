#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2013 all rights reserved
#


"""
Verify that configuration settings are available in the component constructor
"""


def declare():
    import pyre

    class component(pyre.component, family="sample.configuration"):
        """a test component"""
        # properties
        p1 = pyre.properties.str(default="p1")
        p2 = pyre.properties.str(default="p2")

        # behaviors
        @pyre.export
        def do(self):
            """behave"""


        # meta method
        def __init__(self, **kwds):
            super().__init__(**kwds)

            # check that configuration settings have been applied after super().__init__ returns
            # for a specially named component...
            if self.pyre_name == 'c':
                # has a known configuration applied
                assert self.p1 == 'p1 - instance'
                assert self.p2 == 'p2 - instance'
            # while others have access to the default values
            else:
                assert self.p1 == 'sample - p1'
                assert self.p2 == 'sample - p2'

      
    return component


def test():
    # get the declaration
    component = declare()
    # instantiate
    c = component(name="c")
    # check that the configuration setting were transferred correctly
    assert c.p1 == "p1 - instance"
    assert c.p2 == "p2 - instance"
    # now make another with a generic name
    c = component()
    # check that it gets the class defaults
    assert c.p1 == "sample - p1"
    assert c.p2 == "sample - p2"

    # and return the component instance
    return c
    

# main
if __name__ == "__main__":
    test()


# end of file 

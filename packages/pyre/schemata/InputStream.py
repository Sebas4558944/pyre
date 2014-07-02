# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2014 all rights reserved
#


# externals
import sys
# superclasses
from .Schema import Schema
from ..framework.Dashboard import Dashboard


# declaration
class InputStream(Schema, Dashboard):
    """
    A representation of input streams
    """


    # constants
    mode = 'r'
    typename = 'istream'

    # types
    from . import uri
    

    # interface
    def coerce(self, value, **kwds):
        """
        Attempt to convert {value} into an open input stream
        """
        # the value is the special string "stdin"
        if value == 'stdin':
            # return the process stdin
            return sys.stdin
        # if the {value} is either a string or a uri
        if isinstance(value, str) or isinstance(value, self.uri.locator):
            # assume that the framework fileserver knows how to deal with it
            return self.pyre_fileserver.open(uri=value, mode=self.mode)
        # otherwise, leave it alone
        return value
        

    # meta-methods
    def __init__(self, default='stdin', mode=mode, **kwds):
        # chain up, potentially with local default
        super().__init__(default=default, **kwds)
        # save my mode
        self.mode = mode
        # all done
        return


# end of file 

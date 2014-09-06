# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2014 all rights reserved
#


# superclass
from .Sequence import Sequence


# declaration
class Tuple(Sequence):
    """
    The tuple type declarator
    """


    # constants
    typename = 'tuple' # the name of my type
    container = tuple # the container i represent


    # interface
    def coerce(self, value, **kwds):
        """
        Convert {value} into a tuple
        """
        # easy enough; resist the temptation to optimize this by skipping the call to super: we
        # have to coerce every item in the container!
        return self.container(super().coerce(value, **kwds))


# end of file 

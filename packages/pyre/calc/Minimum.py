# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2013 all rights reserved
#


class Minimum:
    """
    The representation of the minimum value of a collection of nodes
    """


    # interface
    def getValue(self):
        """
        Compute and return my value
        """
        # compute and return my value 
        return min(operand.value for operand in self.operands)


# end of file 

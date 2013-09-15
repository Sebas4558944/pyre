# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2013 all rights reserved
#


# access to the pyre package
import pyre
# my ancestor
from .LineMill import LineMill


# my declaration
class TeX(LineMill):
    """
    Support for TeX
    """


    # traits
    languageMarker = pyre.properties.str(default='LaTeX')
    languageMarker.doc = "the TeX variant to use in the language marker"


    # meta methods
    def __init__(self, **kwds):
        super().__init__(comment='%', **kwds)
        return


# end of file 

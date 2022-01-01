# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2022 all rights reserved
#


# exceptions
from ..framework.exceptions import FrameworkError


class ParsingError(FrameworkError):
    """
    Exception raised on failed attempts to convert a string value to one of the primitive types
    """

    # meta-methods
    def __init__(self, value, **kwds):
        # chain up
        super().__init__(**kwds)
        # save the error info
        self.value = value
        # all done
        return


class PathError(FrameworkError):
    """
    Base exception for runtime errors generated by {path}
    """

    # meta-methods
    def __init__(self, path, **kwds):
        # chain up
        super().__init__(**kwds)
        # save the path
        self.path = path
        # all done
        return


class SymbolicLinkLoopError(PathError):
    """
    Exception raised when a loop is detected while resolving a symbolic link
    """

    # public data
    description = "while resolving '{0.path}': symbolic link loop at '{0.loop}'"

    # meta-methods
    def __init__(self, loop, **kwds):
        # chain  up
        super().__init__(**kwds)
        # save the place where the loop was detected
        self.loop = loop
        # all done
        return


# end of file

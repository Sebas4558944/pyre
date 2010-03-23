# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


"""
This package provides support for writing simple parsers
"""


# factories
def token(regexp=None):
    """
    Build a TokenDescriptor instance to hold the regeular expression until the Lexer has had a
    chance to process it and convert it into a class derived from Token
    """
    from .TokenDescriptor import TokenDescriptor
    return TokenDescriptor(regexp)


# exceptions
class ParsingError(Exception):
    """
    Base class for parsing exceptions

    Can be used to catch all exceptions raised by this package
    """

    def __init__(self, description, locator, **kwds):
        super().__init__(**kwds)
        self.description = description
        self.locator = locator
        return

    def __str__(self):
        return "error while parsing {0.locator}: {0.description}".format(self)


class TokenizationError(ParsingError):
    """
    Exception raised when the scanner fails to extract a token from the input stream
    """

    def __init__(self, text, **kwds):
        super().__init__(description="could not match {0!r}".format(text), **kwds)
        self.text = text
        return


# end of file 

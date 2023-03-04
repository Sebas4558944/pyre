# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2023 all rights reserved


# support
import itertools
import journal

# metaclass
from .Schema import Schema

# superclass
from .Descriptor import Descriptor

# parts
from .Inventory import Inventory


# the composite node
class Group(Descriptor, metaclass=Schema):
    """
    A container of datasets
    """

    # metamethods
    def __init__(self, **kwds):
        # chain up
        super().__init__(**kwds)
        # initialize the storage for my dynamic content
        self._pyre_instanceDescriptors = Inventory()
        # all done
        return

    # representation
    def __str__(self) -> str:
        """
        Human readable description
        """
        # unpack my info
        name = self._pyre_name
        # and the info of my type
        cls = type(self)
        module = cls.__module__
        typename = cls.__name__
        # put it all together
        return f"group '{name}', an instance of '{module}.{typename}'"

    # framework hooks
    # introspection
    def _pyre_descriptors(self):
        """
        Generate a sequence of my descriptors
        """
        # throw all descriptor name into a pile
        names = set(
            itertools.chain(self._pyre_instanceDescriptors, self._pyre_classDescriptors)
        )
        # go through them
        for name in names:
            # retrieve the associated value
            attr = getattr(self, name)
            # if it is a descriptor
            if isinstance(attr, Descriptor):
                # we got one; send it off
                yield attr
        # all done
        return

    # visiting
    def _pyre_identify(self, authority, **kwds):
        """
        Let {authority} know i am a group
        """
        # attempt to
        try:
            # ask {authority} for my handler
            handler = authority._pyre_onGroup
        # if it doesn't understand
        except AttributeError:
            # chain up
            return super()._pyre_identify(authority=authority, **kwds)
        # otherwise, invoke the handler
        return handler(group=self, **kwds)

    # decoration
    def _pyre_marker(self):
        """
        Generate an identifying mark for structural renderings
        """
        # easy
        return "g"


# end of file

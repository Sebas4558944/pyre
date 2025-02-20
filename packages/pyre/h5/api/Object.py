# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2023 all rights reserved


# external
import journal

# superclass
from .Location import Location

# typing
import typing
from .. import schema


# a basic h5 object
class Object(Location):
    """
    The base class for all publicly visible h5 objects
    """

    # metamethods
    def __init__(self, layout: typing.Optional[schema.descriptor] = None, **kwds):
        # chain up
        super().__init__(**kwds)
        # record my type
        self._pyre_layout = layout
        # all done
        return

    # framework hooks
    # properties
    @property
    def _pyre_objectType(self):
        """
        Look up my h5 identifier type
        """
        # my handle knows
        return self._pyre_id.objectType

    # visitor
    def _pyre_identify(self, authority, **kwds):
        """
        Let {authority} know i am an object
        """
        # attempt to
        try:
            # ask {authority} for my handler
            handler = authority._pyre_onObject
        # if it doesn't understand
        except AttributeError:
            # chain up
            return super()._pyre_identify(authority=authority, **kwds)
        # otherwise, invoke the handler
        return handler(object=self, **kwds)

    # rendering
    def _pyre_view(self, channel=None, flush=True):
        """
        Generate a textual representation of my structure in a journal {channel}
        """
        # get the explorer factory
        from .Viewer import Viewer as viewer

        # if we don't have a channel
        if channel is None:
            # make one
            channel = journal.info("pyre.h5.object")
        # build the report
        channel.report(report=viewer().visit(self))
        # if we were asked to flush the channel
        if flush:
            # do it
            channel.log()
        # all done
        return


# end of file

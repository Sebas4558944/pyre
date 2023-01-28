# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis <michael.aivazis@para-sim.com>
# (c) 1998-2023 all rights reserved


# typing
from .Dataset import Dataset
from .Group import Group
from .Location import Location


# the visitor
class Explorer:
    """
    A visitor that draws the contents of an h5 location
    """

    # interface
    def visit(self, location: Location):
        """
        Draw the tree at {location}
        """
        # easy enough
        yield from location.pyre_identify(authority=self)
        # all done
        return

    # implementation details
    def pyre_onDataset(self, dataset: Dataset, graphic: str = ""):
        """
        Process a {dataset}
        """
        # get the value
        value = dataset.value
        # the type
        typename = dataset.type
        # the shape
        shape = dataset.shape
        # and the location
        loc = dataset.pyre_name
        # build a representation for the value
        rep = dataset.string(value=value)
        # assemble the label
        yield f"{graphic}{loc} = {rep}, type: {typename}, shape: {shape}"
        # all done
        return

    def pyre_onGroup(self, group: Group, graphic: str = ""):
        """
        Process a group
        """
        # use the {group} location as the label
        yield f"{graphic}{group.pyre_name}:"
        # indent
        graphic += "  "
        # go through the {group} children
        for child in group.pyre_locations():
            # and visit each one
            yield from child.pyre_identify(authority=self, graphic=graphic)
        # all done
        return


# end of file

# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


import pyre
import random
from pyre.components.Component import Component
from ..interfaces.PointCloud import PointCloud


class WichmannHill(Component, family="gauss.generators.wichmann-hill", implements=PointCloud):
    """
    A point generator that uses the random number generator that is part of the python run time
    library to create a point cloud
    """


    @pyre.components.export
    def points(self, count, box):
        """
        Return {count} random points chosen from the interior of {box}
        """
        # unpack the bounding box to form its extent along each of the coördinate axes
        intervals = tuple(zip(*box))
        # our random number generator
        generator = random.uniform
        # loop {count} times
        while count:
            # decrement the counter
            count -= 1
            # build a point by calling the random number generator as many times as there are
            # dimensions in the box specification
            p = tuple(generator(left, right) for left,right in intervals)
            # send it along
            yield p
        # all done
        return


# end of file 

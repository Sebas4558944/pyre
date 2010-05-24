#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


import operator
import functools


def gauss():
    """
    The driver for the generator based implementation
    """
    from Disk import Disk
    from Gaussian import Gaussian
    from MersenneTwister import MersenneTwister

    # inputs
    N = 10**5
    box = [(0,0), (1,1)]
    volume = functools.reduce(operator.mul, ((right-left) for left,right in zip(*box)))
    # the point cloud generator
    generator = MersenneTwister()
    # the region of integration
    disk = Disk(center=(0,0), radius=1)
    # the integrand
    gaussian = Gaussian(mean=(0,0), spread=1/3)

    # the integration algorithm
    # build the point sample
    sample = generator.points(N, box)
    # select the interior points
    interior = disk.interior(sample)
    # compute the integral
    integral = volume/N * sum(gaussian.eval(interior))

    # print out the estimate of #@$\pi$@
    print("integral: {0:.8f}".format(integral))
    return


# main
if __name__ == "__main__":
    gauss()

# end of file 

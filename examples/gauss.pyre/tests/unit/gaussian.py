#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


"""
Sanity check: verify that the Gaussian functor behaves as expected
"""


def test():
    import gauss.functors

    # instantiate
    bell = gauss.functors.gaussian(name="bell")
    bell.μ = [0.0]
    bell.σ = 1.0

    # set up some points
    points = [[-1.], [-.5], [0], [.5], [1.]]
    answers = [0.242, 0.352, 0.399, 0.352, 0.242]
    # exercise the behavior
    for v, check in zip(bell.eval(points), answers):
        assert abs(v-check) < 1.0e-3
    
    return bell


# main
if __name__ == "__main__":
    test()


# end of file 

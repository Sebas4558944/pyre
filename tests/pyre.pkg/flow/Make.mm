# -*- Makefile -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2023 all rights reserved
#


PROJECT = pyre

all: test

test: sanity

sanity:
	${PYTHON} ./sanity.py


# end of file

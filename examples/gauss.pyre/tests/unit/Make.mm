# -*- Makefile -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


PROJECT = gauss.pyre

#--------------------------------------------------------------------------
#

all: test

test: sanity functors generators shapes integrators

sanity:
	${PYTHON} ./sanity.py

functors:
	${PYTHON} ./one.py
	${PYTHON} ./gaussian.py

generators:
	${PYTHON} ./wickmannhill.py

shapes:
	${PYTHON} ./ball.py
	${PYTHON} ./box.py

integrators:
	${PYTHON} ./montecarlo.py


# end of file 

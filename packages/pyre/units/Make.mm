# -*- Makefile -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


PROJECT = pyre
PACKAGE = units
PROJ_DISTCLEAN = $(EXPORT_MODULEDIR)/$(PACKAGE)


#--------------------------------------------------------------------------
#

all: export

#--------------------------------------------------------------------------
# export

EXPORT_PYTHON_MODULES = \
    Dimensional.py \
    Parser.py \
    SI.py \
    angle.py \
    area.py \
    density.py \
    energy.py \
    force.py \
    length.py \
    mass.py \
    power.py \
    pressure.py \
    speed.py \
    substance.py \
    temperature.py \
    time.py \
    volume.py \
    exceptions.py \
    __init__.py


export:: export-package-python-modules

# end of file 

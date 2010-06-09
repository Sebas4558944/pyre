# -*- Makefile -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2010 all rights reserved
#


PROJECT = pyre
PACKAGE = codecs
PROJ_DISTCLEAN = $(EXPORT_MODULEDIR)/$(PACKAGE)

RECURSE_DIRS = \
    native \
    pml \
    pcs \

#--------------------------------------------------------------------------
#

all: export

tidy::
	BLD_ACTION="tidy" $(MM) recurse

clean::
	BLD_ACTION="clean" $(MM) recurse

distclean::
	BLD_ACTION="distclean" $(MM) recurse

#--------------------------------------------------------------------------
# export

EXPORT_PYTHON_MODULES = \
    Codec.py \
    CodecManager.py \
    exceptions.py \
    __init__.py


export:: export-package-python-modules
	BLD_ACTION="export" $(MM) recurse


# end of file 

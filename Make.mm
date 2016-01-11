# -*- Makefile -*-
#
# michael a.g. aïvázis
# orthologue
# (c) 1998-2016 all rights reserved
#


# project global settings
include pyre.def
# my subdirectories
RECURSE_DIRS = \
    lib \
    packages \
    extensions \
    defaults \
    bin \
    templates \
    schema \
    tests \
    examples \
    web \
    bot \
    people

# the standard targets
all:
	BLD_ACTION="all" $(MM) recurse

tidy::
	BLD_ACTION="tidy" $(MM) recurse

clean::
	BLD_ACTION="clean" $(MM) recurse

distclean::
	BLD_ACTION="distclean" $(MM) recurse

live:
	BLD_ACTION="live" $(MM) recurse

# other targets
build: lib packages extensions defaults bin

test: build tests examples

# the pyre install archives
PYTHON_TAG = ${shell $(PYTHON) bin/cache_tag.py}
PYRE_VERSION = $(PROJECT_MAJOR).$(PROJECT_MINOR)
PYRE_BOOTPKGS = pyre journal merlin
PYRE_ZIP = $(EXPORT_ROOT)/pyre-$(PYRE_VERSION).$(PYTHON_TAG).zip
PYRE_BOOTZIP = $(EXPORT_ROOT)/pyre-$(PYRE_VERSION)-boot.$(PYTHON_TAG).zip

zip: build zipit

zipit:
	$(RM_F) $(PYRE_ZIP)
	(cd $(EXPORT_ROOT); zip -r ${PYRE_ZIP} * )

boot:
	@$(RM_F) $(PYRE_BOOTZIP)
	@(cd $(EXPORT_ROOT)/packages; zip -r ${PYRE_BOOTZIP} $(PYRE_BOOTPKGS)--include \*.pyc)
	scp $(PYRE_BOOTZIP) $(PROJ_LIVE_USERURL):$(PROJ_LIVE_DOCROOT)
	@$(RM_F) $(PYRE_BOOTZIP)

# shortcuts for building specific subdirectories
.PHONY: $(RECURSE_DIRS) doc

$(RECURSE_DIRS) doc:
	(cd $@; $(MM))


# end of file

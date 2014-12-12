# -*- Makefile -*-
#
# {project.authors}
# {project.affiliations}
# (c) {project.span} all rights reserved
#

# access the project defaults
include {project.name}.def
# the package name
PACKAGE = components

# the list of python modules
EXPORT_PYTHON_MODULES = \
    Action.py \
    Plexus.py \
    __init__.py

# the standard build targets
all: export

export:: export-package-python-modules

# end of file

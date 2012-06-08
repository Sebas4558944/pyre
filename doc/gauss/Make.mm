# -*- Makefile -*-
#
# michael a.g. aïvázis
# california institute of technology
# (c) 1998-2012 all rights reserved
#

PROJECT = pyre
PACKAGE = doc/gauss

RECURSE_DIRS = \
   diagrams \
   figures \
   listings \

OTHERS = \

#--------------------------------------------------------------------------
#

DOCUMENT = gauss

INCLUDES = \
    titlepage.sty \
    config.tex \
    macros.tex \
    meta.tex \
    references.bib

SECTIONS = \
    $(DOCUMENT).tex \
    sec_*.tex \

FIGURES = \
    figures/*.pdf \

LISTINGS = \
    listings/simple/gauss.py \
    listings/simple/gauss.cc \
    listings/classes/*.py \
    listings/containers/*.py \
    listings/generators/*.py \
    listings/components/*.py \

#--------------------------------------------------------------------------
#

all: $(DOCUMENT).pdf clean
	BLD_ACTION="all" $(MM) recurse

tidy::
	BLD_ACTION="tidy" $(MM) recurse

clean::
	BLD_ACTION="clean" $(MM) recurse

distclean::
	BLD_ACTION="distclean" $(MM) recurse


#--------------------------------------------------------------------------
#

# preview types
osx: $(DOCUMENT).pdf
	open $(DOCUMENT).pdf

xpdf: $(DOCUMENT).pdf
	xpdf -remote $(DOCUMENT) $(DOCUMENT).pdf

# make the document using the default document class
$(DOCUMENT).pdf: $(DOCUMENT).tex $(INCLUDES) $(SECTIONS) $(FIGURES) $(LISTINGS)

# housekeeping
PROJ_CLEAN = $(CLEAN_LATEX) *.snm *.nav *.vrb 
PROJ_DISTCLEAN = *.ps *.pdf $(PROJ_CLEAN)

# end of file

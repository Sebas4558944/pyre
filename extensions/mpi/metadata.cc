// -*- C++ -*-
// 
// michael a.g. aïvázis
// california institute of technology
// (c) 1998-2011 all rights reserved
// 

#include <portinfo>
#include <Python.h>

#include "metadata.h"


// copyright
const char * const pyre::extensions::mpi::copyright__name__ = "copyright";
const char * const pyre::extensions::mpi::copyright__doc__ = "the module copyright string";
PyObject * 
pyre::extensions::mpi::
copyright(PyObject *, PyObject *)
{
    const char * const copyright_note = "mpi: (c) 1998-2011 Michael A.G. Aïvázis";
    return Py_BuildValue("s", copyright_note);
}
    

// version
const char * const pyre::extensions::mpi::version__name__ = "version";
const char * const pyre::extensions::mpi::version__doc__ = "the module version string";
PyObject * 
pyre::extensions::mpi::
version(PyObject *, PyObject *)
{
    const char * const version_string = "0.0";
    return Py_BuildValue("s", version_string);
}

    
// end of file

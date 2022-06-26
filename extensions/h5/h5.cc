// -*- c++ -*-
//
// michael a.g. aïvázis <michael.aivazis@para-sim.com>
// (c) 1998-2022 all rights reserved


// external dependencies
#include "external.h"
// namespace setup
#include "forward.h"


// the module entry point
PYBIND11_MODULE(h5, m)
{
    // the docstring
    m.doc() = "the hdf5 extension module";

    // register the module api
    pyre::h5::py::api(m);

    // bindings
    pyre::h5::py::dataset(m);
    pyre::h5::py::datatypes::datatypes(m);
    pyre::h5::py::file(m);

    // all done
    return;
}


// end of file

// -*- c++ -*-
//
// michael a.g. aïvázis <michael.aivazis@para-sim.com>
// (c) 1998-2023 all rights reserved

// code guard
#if !defined(h5_py_forward_h)
#define h5_py_forward_h


// the {h5} extension namespace
namespace pyre::h5::py {
    // the module api
    void api(py::module &);

    // wrappers for the C++ HDF5 api
    void enums(py::module &);
    void dataspace(py::module &);
    void dataset(py::module &);
    void fapl(py::module &);
    void file(py::module &);
    void group(py::module &);
} // namespace pyre::h5::py


#endif

// end of file

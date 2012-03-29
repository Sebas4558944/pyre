// -*- C++ -*-
//
// michael a.g. aïvázis
// california institute of technology
// (c) 1998-2012 all rights reserved
//


#if !defined(pyre_extensions_mpi_communicators_h_)
#define pyre_extensions_mpi_communicators_h_


// place everything in my private namespace
namespace mpi {
    namespace communicator {

        // the predefined communicators
        extern PyObject * world;

        // create a communicator (MPI_Comm_create)
        extern const char * const create__name__;
        extern const char * const create__doc__;
        PyObject * create(PyObject *, PyObject *);

        // return the communicator size (MPI_Comm_size)
        extern const char * const size__name__;
        extern const char * const size__doc__;
        PyObject * size(PyObject *, PyObject *);

        // return the process rank in a given communicator (MPI_Comm_rank)
        extern const char * const rank__name__;
        extern const char * const rank__doc__;
        PyObject * rank(PyObject *, PyObject *);

        // set a communicator barrier (MPI_Barrier)
        extern const char * const barrier__name__;
        extern const char * const barrier__doc__;
        PyObject * barrier(PyObject *, PyObject *);
    } // of namespace communicator

    namespace cartesian {
        // create a cartesian communicator (MPI_Cart_create)
        extern const char * const create__name__;
        extern const char * const create__doc__;
        PyObject * create(PyObject *, PyObject *);

        // return the coordinates of the process in the cartesian communicator (MPI_Cart_coords)
        extern const char * const coordinates__name__;
        extern const char * const coordinates__doc__;
        PyObject * coordinates(PyObject *, PyObject *);
    } // of namespace cartesian

} // of namespace mpi

#endif

// end of file

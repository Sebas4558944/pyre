// -*- coding: utf-8 -*-
//
// bianca giovanardi
// (c) 2021 all rights reserved
//

// support
#include <cassert>

// get the tensor algebra
#include <pyre/algebra/tensor_algebra.h>

// use namespace for readability
using namespace pyre::algebra;

// main program
int main(int argc, char* argv[]) {

    // a matrix
    constexpr matrix_t<3, 3> A = { 1, -2, 0, 0, 1, 2, 0, 1, 1 };
    
    // the matrix transpose
    constexpr auto At = transpose(A);

    // transpose of transpose is the identity operator
    static_assert(transpose(At) == A);

    // transpose preserves trace
    static_assert(tr(At) == tr(A));

    // transpose preserves determinant
    static_assert(det(At) == det(A));

    // all done
    return 0;
}

// end of file

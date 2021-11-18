// -*- coding: utf-8 -*-
//
// bianca giovanardi
// (c) 2021 all rights reserved
//

// support
#include <cassert>

// get the tensor algebra
#include <pyre/algebra.h>

// use namespace for readability
using namespace pyre::algebra;

// main program
int main(int argc, char* argv[]) {

    // 2D vector: canonical basis
    static_assert(VectorBasis<2>::unit<0> == vector_t<2>{1, 0});
    static_assert(VectorBasis<2>::unit<1> == vector_t<2>{0, 1});
    static_assert(VectorBasis<2>::unit<0> + VectorBasis<2>::unit<1> == vector_t<2>::one);

    // 3D vector: canonical basis
    static_assert(VectorBasis<3>::unit<0> == vector_t<3>{1, 0, 0});
    static_assert(VectorBasis<3>::unit<1> == vector_t<3>{0, 1, 0});
    static_assert(VectorBasis<3>::unit<2> == vector_t<3>{0, 0, 1});
    static_assert(VectorBasis<3>::unit<0> + VectorBasis<3>::unit<1> + VectorBasis<3>::unit<2> 
        == vector_t<3>::one);

    // 2D matrix: canonical basis
    static_assert(MatrixBasis<2>::unit<0, 0> == matrix_t<2>{1, 0, 0, 0});
    static_assert(MatrixBasis<2>::unit<0, 1> == matrix_t<2>{0, 1, 0, 0});
    static_assert(MatrixBasis<2>::unit<1, 0> == matrix_t<2>{0, 0, 1, 0});
    static_assert(MatrixBasis<2>::unit<1, 1> == matrix_t<2>{0, 0, 0, 1});
    static_assert(
          MatrixBasis<2>::unit<0, 0> + MatrixBasis<2>::unit<0, 1>
        + MatrixBasis<2>::unit<1, 0> + MatrixBasis<2>::unit<1, 1>
        == matrix_t<2, 2>::one);

    // 3D matrix: canonical basis
    static_assert(MatrixBasis<3>::unit<0, 0> == matrix_t<3>{1, 0, 0, 0, 0, 0, 0, 0, 0});
    static_assert(MatrixBasis<3>::unit<0, 1> == matrix_t<3>{0, 1, 0, 0, 0, 0, 0, 0, 0});
    static_assert(MatrixBasis<3>::unit<0, 2> == matrix_t<3>{0, 0, 1, 0, 0, 0, 0, 0, 0});
    static_assert(MatrixBasis<3>::unit<1, 0> == matrix_t<3>{0, 0, 0, 1, 0, 0, 0, 0, 0});
    static_assert(MatrixBasis<3>::unit<1, 1> == matrix_t<3>{0, 0, 0, 0, 1, 0, 0, 0, 0});
    static_assert(MatrixBasis<3>::unit<1, 2> == matrix_t<3>{0, 0, 0, 0, 0, 1, 0, 0, 0});
    static_assert(MatrixBasis<3>::unit<2, 0> == matrix_t<3>{0, 0, 0, 0, 0, 0, 1, 0, 0});
    static_assert(MatrixBasis<3>::unit<2, 1> == matrix_t<3>{0, 0, 0, 0, 0, 0, 0, 1, 0});
    static_assert(MatrixBasis<3>::unit<2, 2> == matrix_t<3>{0, 0, 0, 0, 0, 0, 0, 0, 1});
    static_assert(
          MatrixBasis<3>::unit<0, 0> + MatrixBasis<3>::unit<0, 1> + MatrixBasis<3>::unit<0, 2>
        + MatrixBasis<3>::unit<1, 0> + MatrixBasis<3>::unit<1, 1> + MatrixBasis<3>::unit<1, 2>
        + MatrixBasis<3>::unit<2, 0> + MatrixBasis<3>::unit<2, 1> + MatrixBasis<3>::unit<2, 2>
        == matrix_t<3, 3>::one);

    // 2D vector: basis vectors are orthogonal
    static_assert(VectorBasis<2>::unit<0> * VectorBasis<2>::unit<1> == 0);

    // 3D vector: basis vectors are orthogonal
    static_assert(VectorBasis<3>::unit<0> * VectorBasis<3>::unit<1> == 0);
    static_assert(VectorBasis<3>::unit<0> * VectorBasis<3>::unit<2> == 0);
    static_assert(VectorBasis<3>::unit<1> * VectorBasis<3>::unit<2> == 0);

    // all done
    return 0;
}

// end of file

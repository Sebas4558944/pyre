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

    // vector: zero + one == one
    static_assert(vector_t<2>::zero + vector_t<2>::one == vector_t<2>::one);
    static_assert(vector_t<3>::zero + vector_t<3>::one == vector_t<3>::one);

    // vector: zero == one - one
    static_assert(vector_t<2>::zero == vector_t<2>::one - vector_t<2>::one);
    static_assert(vector_t<3>::zero == vector_t<3>::one - vector_t<3>::one);

    // matrix: zero + one == one
    static_assert(matrix_t<2, 2>::zero + matrix_t<2, 2>::one == matrix_t<2, 2>::one);
    static_assert(matrix_t<3, 3>::zero + matrix_t<3, 3>::one == matrix_t<3, 3>::one);

    // matrix: zero == one - one
    static_assert(matrix_t<2, 2>::zero == matrix_t<2, 2>::one - matrix_t<2, 2>::one);
    static_assert(matrix_t<3, 3>::zero == matrix_t<3, 3>::one - matrix_t<3, 3>::one);

    // a scalar with negative one 
    constexpr scalar_t minus_one = -1.0;

    // vector: (-1) * v == - v
    static_assert(minus_one * vector_t<2>::one == -vector_t<2>::one);
    static_assert(vector_t<2>::one * minus_one == -vector_t<2>::one);
    static_assert(minus_one * vector_t<3>::one == -vector_t<3>::one);
    static_assert(vector_t<3>::one * minus_one == -vector_t<3>::one);

    // matrix: (-1) * A == - A
    static_assert(minus_one * matrix_t<2, 2>::one == -matrix_t<2, 2>::one);
    static_assert(matrix_t<2, 2>::one * minus_one == -matrix_t<2, 2>::one);
    static_assert(minus_one * matrix_t<3, 3>::one == -matrix_t<3, 3>::one);
    static_assert(matrix_t<3, 3>::one * minus_one == -matrix_t<3, 3>::one);

    // a 2D vector
    constexpr vector_t<2> vector2D { -2.0, 2.0 };

    // v + v == 2*v 
    static_assert(vector2D + vector2D == 2.0 * vector2D);

    // ( v + v ) / 2 == v 
    static_assert((vector2D + vector2D) / 2.0 == vector2D);

    // ( v + v ) / -2 == -v 
    static_assert((vector2D + vector2D) / -2.0 == -vector2D);

    // a 3D vector
    constexpr vector_t<3> vector3D { -2.0, 2.0, 10.0 };

    // v + v + v == 3 * v 
    static_assert(vector3D + vector3D + vector3D == 3.0 * vector3D);

    // ( v + v + v ) / 3 == v 
    static_assert((vector3D + vector3D + vector3D) / 3.0 == vector3D);

    // ( v + v + v ) / -3 == - v 
    static_assert((vector3D + vector3D + vector3D) / -3.0 == -vector3D);

    // all done
    return 0;
}

// end of file

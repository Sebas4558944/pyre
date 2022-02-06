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

    // Packing-independent base for 2D matrix 
    static constexpr auto e_00 = MatrixBasis<2>::unit<0, 0>;
    static constexpr auto e_01 = MatrixBasis<2>::unit<0, 1>;
    static constexpr auto e_10 = MatrixBasis<2>::unit<1, 0>;
    static constexpr auto e_11 = MatrixBasis<2>::unit<1, 1>;
    static constexpr auto e_01s = symmetric(MatrixBasis<2>::unit<0, 1> 
                                            + MatrixBasis<2>::unit<1, 0>);

    // a 2D matrix
    static constexpr auto A = true * e_00 + 2.0 * e_01 + 3.0 * e_10 + 4 * e_11;

    // a 2D symmetric matrix
    static constexpr auto B = 1.0 * e_00 + 2.0 * e_01s + 4.0 * e_11;

    // a 2D diagonal matrix
    static constexpr auto C = 1.0 * e_00 + 2.0 * e_11;

    // check the math
    static_assert(A + B + C == 3.0 * e_00 + 4.0 * e_01 + 5.0 * e_10 + 10.0 * e_11);
    static_assert(B + C + A == A + B + C);
    static_assert(C + A + B == A + B + C);

    // all done
    return 0;
}


// end of file

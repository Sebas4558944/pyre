// -*- c++ -*-
//
// michael a.g. aïvázis <michael.aivazis@para-sim.com>
// (c) 1998-2021 all rights reserved

// code guard
#if !defined(pyre_viz_forward_h)
#define pyre_viz_forward_h


// set up the namespace
namespace pyre::viz {

    // just to make sure we are all on the same page
    using byte_t = char;

    // individual color values are floats in [0,1]
    using color_t = float;
    // {r,g,b} triplets
    using rgb_t = std::tuple<color_t, color_t, color_t>;

    // codecs
    // micorosoft bitmap
    class BMP;

    // color maps
    namespace colormaps {
        template <class sourceT>
        class Complex;
    }

} // namespace pyre::viz


#endif

// end of file

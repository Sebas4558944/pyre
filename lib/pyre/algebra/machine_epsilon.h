// -*- c++ -*-
//
// bianca giovanardi
// (c) 2021 all rights reserved


#include <cfloat>
#include <cmath>


// returns the (positive) distance between x and the next representable number larger than x
template<typename T>
constexpr T epsilon_right (T x)
{
    return std::nextafter(x, std::numeric_limits<T>::max()) - x; 
}

// returns the (positive) distance between x and the next representable number smaller than x
template<typename T>
constexpr T epsilon_left (T x)
{
    return x - std::nextafter(x, std::numeric_limits<T>::min());
}

template<typename T>
constexpr T epsilon (T x)
{
    return std::max(epsilon_left(x), epsilon_right(x));
}


// end of file

#pragma once

#include "types.hh"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <type_traits>

namespace cherry_blazer::detail {

// Taken from: https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
// ULP=4 was chosen arbitrarily as common value.
template <class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y,
                                                                                      u8 ulp = 4) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
           // unless the result is subnormal
           || std::fabs(x - y) < std::numeric_limits<T>::min();
}
// TODO: fabs() is not constexpr

[[noreturn]] inline void unreachable(char const* msg) {
    std::cerr << "Unreachable code reached: " << msg << std::endl;
    std::terminate();
}

} // namespace cherry_blazer::detail

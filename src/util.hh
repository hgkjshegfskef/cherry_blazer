#ifndef CHERRY_BLAZER_SRC_UTIL_HH_
#define CHERRY_BLAZER_SRC_UTIL_HH_

#include <cmath>
#include <limits>
#include <type_traits>

// Taken from: https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
// ULP=4 was chosen arbitrarily as common value.
template <class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y,
                                                                                      int ulp = 4) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
           // unless the result is subnormal
           || std::fabs(x - y) < std::numeric_limits<T>::min();
}

#endif // CHERRY_BLAZER_SRC_UTIL_HH_

#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_OPTIONAL_DOUBLE_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_OPTIONAL_DOUBLE_HH_

#include <ak_toolkit/markable.hpp>

namespace cherry_blazer {

// Markable doesn't occupy twice the size like std::optional.
using optional_double = ak_toolkit::markable<ak_toolkit::markable_ns::mark_fp_nan<double>>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_OPTIONAL_DOUBLE_HH_

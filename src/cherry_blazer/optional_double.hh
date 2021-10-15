#pragma once

#include <ak_toolkit/markable.hpp>

namespace cherry_blazer {

// Markable doesn't occupy twice the size like std::optional.
using optional_double = ak_toolkit::markable<ak_toolkit::markable_ns::mark_fp_nan<double>>;

} // namespace cherry_blazer

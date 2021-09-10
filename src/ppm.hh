#ifndef CHERRY_BLAZER_SRC_PPM_HH_
#define CHERRY_BLAZER_SRC_PPM_HH_

#include "safe_numerics_typedefs.hh"
#include "types.hh"

#include <string>

namespace cherry_blazer::ppm {

std::string generate_header(safe_auto<u32> const& width, safe_auto<u32> const& height,
                            safe_auto<u32> const& color_component_max);

} // namespace cherry_blazer::ppm

#endif // CHERRY_BLAZER_SRC_PPM_HH_

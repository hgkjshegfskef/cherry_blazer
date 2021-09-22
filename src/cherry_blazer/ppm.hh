#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_PPM_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_PPM_HH_

#include "types.hh"

#include <string>

namespace cherry_blazer::ppm {

std::string generate_header(u32 width, u32 height, u32 color_component_max);

} // namespace cherry_blazer::ppm

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_PPM_HH_

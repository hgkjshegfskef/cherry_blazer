#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_PPM_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_PPM_HH_

#include "detail/types.hh"

#include <string>

namespace cherry_blazer::ppm {

std::string generate_header(std::size_t width, std::size_t height, std::size_t color_component_max);

} // namespace cherry_blazer::ppm

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_PPM_HH_

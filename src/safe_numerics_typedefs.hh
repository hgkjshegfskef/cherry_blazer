#ifndef CHERRY_BLAZER_SRC_SAFE_NUMERICS_TYPEDEFS_HH_
#define CHERRY_BLAZER_SRC_SAFE_NUMERICS_TYPEDEFS_HH_

#include <boost/safe_numerics/automatic.hpp>
#include <boost/safe_numerics/safe_integer.hpp>
#include <boost/safe_numerics/safe_integer_literal.hpp>
#include <boost/safe_numerics/safe_integer_range.hpp>

namespace cherry_blazer {

template <std::uintmax_t u>
using safe_uliteral_auto =
    boost::safe_numerics::safe_unsigned_literal<u, boost::safe_numerics::automatic>;

template <std::uintmax_t u_begin, std::uintmax_t u_end>
using safe_urange_auto =
    boost::safe_numerics::safe_unsigned_range<u_begin, u_end, boost::safe_numerics::automatic>;

template <typename Type>
using safe_auto = boost::safe_numerics::safe<Type, boost::safe_numerics::automatic>;

template <std::uintmax_t u>
using safe_uliteral_auto_trap =
    boost::safe_numerics::safe_unsigned_literal<u, boost::safe_numerics::automatic,
                                                boost::safe_numerics::loose_trap_policy>;

template <std::uintmax_t u_begin, std::uintmax_t u_end>
using safe_urange_auto_trap =
    boost::safe_numerics::safe_unsigned_range<u_begin, u_end, boost::safe_numerics::automatic,
                                              boost::safe_numerics::loose_trap_policy>;

template <typename Type>
using safe_auto_trap = boost::safe_numerics::safe<Type, boost::safe_numerics::automatic,
                                                  boost::safe_numerics::loose_trap_policy>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_SAFE_NUMERICS_TYPEDEFS_HH_

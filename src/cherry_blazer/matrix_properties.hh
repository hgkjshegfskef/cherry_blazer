#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_PRIVATE_MATRIX_PROPERTIES_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_PRIVATE_MATRIX_PROPERTIES_HH_

#include "detail/detail.hh"

#include <boost/io/ios_state.hpp>

#include <algorithm>
#include <cstddef>
#include <limits>
#include <ostream>

namespace cherry_blazer {

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension> class Matrix;

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
constexpr bool operator==(Matrix<Precision, OuterDimension, InnerDimension> const& lhs,
                          Matrix<Precision, OuterDimension, InnerDimension> const& rhs) noexcept {
    // TODO: almost_equal is not constexpr
    return std::equal(
        std::cbegin(lhs), std::cend(lhs), std::cbegin(rhs), std::cend(rhs),
        [](auto const& lhs, auto const& rhs) { return detail::almost_equal(lhs, rhs); });
}

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
constexpr bool operator!=(Matrix<Precision, OuterDimension, InnerDimension> const& lhs,
                          Matrix<Precision, OuterDimension, InnerDimension> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
std::ostream& operator<<(std::ostream& os,
                         Matrix<Precision, OuterDimension, InnerDimension> const& mat) noexcept {
    boost::io::ios_precision_saver ips{os};
    os.precision(
        std::numeric_limits<
            typename Matrix<Precision, OuterDimension, InnerDimension>::value_type>::max_digits10);

    for (auto row{0U}; row < OuterDimension - 1; ++row) {
        for (auto col{0U}; col < InnerDimension - 1; ++col) {
            os << mat(row, col) << '\t';
        }
        os << mat(row, InnerDimension - 1) << '\n';
    }

    for (auto col{0U}; col < InnerDimension - 1; ++col) {
        os << mat(OuterDimension - 1, col) << '\t';
    }
    os << mat(OuterDimension - 1, InnerDimension - 1);

    ips.restore();

    return os;
}

template <std::size_t Ith, std::size_t Jth, typename Precision, std::size_t OuterDimension,
          std::size_t InnerDimension>
constexpr typename Matrix<Precision, OuterDimension, InnerDimension>::reference
get(Matrix<Precision, OuterDimension, InnerDimension>& mat) noexcept {
    static_assert(0 <= Ith && Ith < OuterDimension, "Matrix outer index is out of bounds.");
    static_assert(0 <= Jth && Jth < InnerDimension, "Matrix inner index is out of bounds.");
    return mat(Ith, Jth);
}
template <std::size_t Ith, std::size_t Jth, typename Precision, std::size_t OuterDimension,
          std::size_t InnerDimension>
constexpr typename Matrix<Precision, OuterDimension, InnerDimension>::const_reference
get(Matrix<Precision, OuterDimension, InnerDimension> const& mat) noexcept {
    static_assert(0 <= Ith && Ith < OuterDimension, "Matrix outer index is out of bounds.");
    static_assert(0 <= Jth && Jth < InnerDimension, "Matrix inner index is out of bounds.");
    return mat(Ith, Jth);
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_PRIVATE_MATRIX_PROPERTIES_HH_

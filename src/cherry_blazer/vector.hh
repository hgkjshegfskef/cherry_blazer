#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_VECTOR_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_VECTOR_HH_

#include "matrix.hh"

#include <boost/assert.hpp>

namespace cherry_blazer {

// Nx1 matrix, a.k.a. Vector.
template <typename Precision, std::size_t OuterDimension>
class Matrix<Precision, OuterDimension, 1>
    // 2D Vector is always 3-dimensional, 3D Vector is always 4-dimensional, with the last component
    // set to 1, even though their types remain 2,1 and 3,1 respectively. This is useful when a
    // matrix representing an affine transformation (e.g. a 4D matrix) is multiplied by this vector
    // (e.g. 3D vector), which would otherwise need to be augmented to support representation in
    // homogeneous coordinates.
    : public detail::MatrixBase<Precision, OuterDimension + 1, 1> {

    using base = detail::MatrixBase<Precision, OuterDimension + 1, 1>;
    using typename base::impl;

  public:
    static inline constexpr std::size_t size = OuterDimension;

    Matrix() = default;

    template <typename... VectorComponents> constexpr Matrix(VectorComponents... components) {
        constexpr auto dimension = sizeof...(components);
        static_assert(2 <= dimension && dimension <= 3,
                      "Vector must be constructed with either 2 or 3 components.");
        impl::mat_ = {components...};
        impl::mat_[dimension] = static_cast<Precision>(1);
    }

    constexpr typename base::reference at(typename base::size_type pos) {
        return impl::mat_.at(pos);
    }
    [[nodiscard]] constexpr typename base::const_reference at(typename base::size_type pos) const {
        return impl::mat_.at(pos);
    }

    constexpr typename base::reference operator[](typename base::size_type pos) noexcept {
        // TODO: replace with https://github.com/gpakosz/PPK_ASSERT
        BOOST_VERIFY(pos < OuterDimension + 1);
        return impl::mat_[pos];
    }
    [[nodiscard]] constexpr typename base::const_reference
    operator[](typename base::size_type pos) const noexcept {
        BOOST_VERIFY(pos < OuterDimension + 1);
        return impl::mat_[pos];
    }
};

// Vector and Matrix names can be used interchangeably (for semantic usability).
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension = 1>
using Vector = // NOLINT(readability-identifier-naming)
    Matrix<Precision, OuterDimension, InnerDimension>;

// In C++20 we have class template argument deduction consider aliases (which significantly improves
// usability with Vector type alias). This is implemented in GCC 10, yet to be implemented by Clang,
// as of Clang 12. https://wg21.link/P1814R0 : __cpp_deduction_guides >= 201907
template <typename... VectorComponents>
Matrix(VectorComponents...)
    -> Matrix<typename std::common_type_t<VectorComponents...>, sizeof...(VectorComponents), 1>;

using Vec2f = Matrix<float, 2, 1>;  // NOLINT(readability-identifier-naming)
using Vec3f = Matrix<float, 3, 1>;  // NOLINT(readability-identifier-naming)
using Vec2d = Matrix<double, 2, 1>; // NOLINT(readability-identifier-naming)
using Vec3d = Matrix<double, 3, 1>; // NOLINT(readability-identifier-naming)

// This is actually a Vector, but it cannot be named.
extern template class Matrix<double, 3, 1>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_VECTOR_HH_

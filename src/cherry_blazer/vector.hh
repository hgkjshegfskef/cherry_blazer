#pragma once

#include "detail/matrix_base.hh"

#include <boost/assert.hpp>

namespace cherry_blazer {

// Forward declarations
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension> class Matrix;
template <typename Precision, std::size_t Dimension> class Point;

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

    template <typename... VectorComponents,
              typename = std::enable_if_t<
                  // Do not shadow Vector::Vector(Point const&, Point const&)
                  not std::is_same_v<std::common_type_t<VectorComponents...>,
                                     std::decay_t<Point<Precision, OuterDimension>>> and
                  // Do not shadow Matrix::Matrix(Matrix const&) and Matrix::Matrix(Matrix&&)
                  not(sizeof...(VectorComponents) == 1 and
                      std::is_same_v<std::common_type_t<VectorComponents...>,
                                     std::decay_t<Matrix<Precision, OuterDimension, 1>>>)>>
    constexpr explicit Matrix(VectorComponents&&... components) {
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

template <typename Precision, std::size_t OuterDimension>
class Vector : public Matrix<Precision, OuterDimension, 1> {
  public:
    Vector() = default;
    using Matrix<Precision, OuterDimension, 1>::Matrix;

    constexpr Vector(Point<Precision, OuterDimension> const& start,
                     Point<Precision, OuterDimension> const& end) noexcept;
};

template <typename First, typename... Rest,
          typename = std::enable_if_t<(std::is_same_v<First, Rest> && ...)>>
Vector(First, Rest...) -> Vector<First, 1 + sizeof...(Rest)>;

using Vec2f = Vector<float, 2>;  // NOLINT(readability-identifier-naming)
using Vec3f = Vector<float, 3>;  // NOLINT(readability-identifier-naming)
using Vec2d = Vector<double, 2>; // NOLINT(readability-identifier-naming)
using Vec3d = Vector<double, 3>; // NOLINT(readability-identifier-naming)

extern template class Vector<float, 3>;
extern template class Vector<double, 3>;

} // namespace cherry_blazer

// Due to circular dependencies, properly include Point only now to implement
// Vector::Vector(Point const&, Point const&)

#include "point.hh"
#include "point_operations.hh"

namespace cherry_blazer {

template <typename Precision, std::size_t Dimension>
constexpr Vector<Precision, Dimension>::Vector(Point<Precision, Dimension> const& start,
                                               Point<Precision, Dimension> const& end) noexcept
    : Vector(end - start) {}

} // namespace cherry_blazer

#include "matrix_properties.hh"

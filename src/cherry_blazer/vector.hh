// Use ifdefs instead of pragma in this file, because due to circular dependencies IDE thinks there
// is redefinition.
#ifndef CHERRY_BLAZER_VECTOR_HH
#define CHERRY_BLAZER_VECTOR_HH

#include "detail/matrix_base.hh"

#include <boost/assert.hpp>

namespace cherry_blazer {

// Forward declarations
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension> class Matrix;
template <typename Precision, std::size_t Dimension> class Point;

// Nx1 matrix specialization, used later for Vector.
template <typename Precision, std::size_t Dimension>
class Matrix<Precision, Dimension, 1>
    // 2D Vector is always 3-dimensional, 3D Vector is always 4-dimensional.
    : public detail::MatrixBase<Precision, Dimension + 1, 1> {
    using base = detail::MatrixBase<Precision, Dimension + 1, 1>;
};

template <typename Precision, std::size_t Dimension>
class Vector : public Matrix<Precision, Dimension, 1> {

    using base = Matrix<Precision, Dimension, 1>;
    using typename base::impl;

  public:
    Vector() = default;

    constexpr Vector(Point<Precision, Dimension> const& start,
                     Point<Precision, Dimension> const& end) noexcept;

    template <typename... VectorComponents,
              typename = std::enable_if_t<
                  // Do not shadow Vector's ctor from Points.
                  not std::is_same_v<std::common_type_t<VectorComponents...>,
                                     std::decay_t<Point<Precision, Dimension>>> and
                  // Do not shadow Vector's copy and move ctors.
                  not(sizeof...(VectorComponents) == 1 and
                      std::is_same_v<std::common_type_t<VectorComponents...>,
                                     std::decay_t<Vector<Precision, Dimension>>>)>>
    constexpr explicit Vector(VectorComponents&&... components) noexcept {
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
        BOOST_VERIFY(pos < Dimension + 1);
        return impl::mat_[pos];
    }
    [[nodiscard]] constexpr typename base::const_reference
    operator[](typename base::size_type pos) const noexcept {
        BOOST_VERIFY(pos < Dimension + 1);
        return impl::mat_[pos];
    }

    template <typename base::size_type Idx> constexpr typename base::reference get() noexcept {
        static_assert(0 <= Idx && Idx < Dimension, "Vector index is out of bounds.");
        return std::get<Idx>(impl::mat_);
    }
    template <typename base::size_type Idx>
    [[nodiscard]] constexpr typename base::const_reference get() const noexcept {
        static_assert(0 <= Idx && Idx < Dimension, "Vector index is out of bounds.");
        return std::get<Idx>(impl::mat_);
    }
};

template <std::size_t Idx, typename Precision, std::size_t Dimension>
constexpr typename Vector<Precision, Dimension>::reference
get(Vector<Precision, Dimension>& vec) noexcept {
    return vec.template get<Idx>();
}

template <std::size_t Idx, typename Precision, std::size_t Dimension>
constexpr typename Vector<Precision, Dimension>::const_reference
get(Vector<Precision, Dimension> const& vec) noexcept {
    return vec.template get<Idx>();
}

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

#endif // CHERRY_BLAZER_VECTOR_HH

#include "matrix_properties.hh"

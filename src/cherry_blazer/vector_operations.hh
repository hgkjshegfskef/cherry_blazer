#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_VECTOR_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_VECTOR_OPERATIONS_HH_

//#include "point.hh"
#include "vector.hh"

#include <cmath>
#include <iostream>

namespace cherry_blazer {

// -Vector
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Vector<Precision, Dimension> const& v) noexcept {
    std::remove_cvref_t<decltype(v)> result;

    for (auto i{0U}; i < Dimension; ++i)
        result[i] = -v[i];
    result[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(result[Dimension] == static_cast<Precision>(1));
    return result;
}

// Vector*scalar
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator*(Vector<Precision, Dimension> const& v,
                                       Precision scalar) noexcept {
    std::remove_cvref_t<decltype(v)> result;

    for (auto i{0U}; i < Dimension; ++i)
        result[i] = v[i] * scalar;
    result[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(result[Dimension] == static_cast<Precision>(1));
    return result;
}

// scalar*Vector
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator*(Precision scalar,
                                       Vector<Precision, Dimension> const& v) noexcept {
    return v * scalar;
}

// Vector/scalar
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator/(Vector<Precision, Dimension> const& v,
                                       Precision scalar) noexcept {
    std::remove_cvref_t<decltype(v)> result;

    for (auto i{0U}; i < Dimension; ++i)
        result[i] = v[i] / scalar;
    result[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(result[Dimension] == static_cast<Precision>(1));
    return result;
}

// scalar/Vector (= ERROR)

// Vector += Vector (= Vector)
template <typename Precision, std::size_t Dimension>
constexpr auto& operator+=(Vector<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    for (auto i{0U}; i < Dimension; ++i)
        lhs[i] += rhs[i];
    lhs[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(lhs[Dimension] == static_cast<Precision>(1));
    return lhs;
}

// Vector -= Vector (= Vector)
template <typename Precision, std::size_t Dimension>
constexpr auto& operator-=(Vector<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    for (auto i{0U}; i < Dimension; ++i)
        lhs[i] -= rhs[i];
    lhs[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(lhs[Dimension] == static_cast<Precision>(1));
    return lhs;
}

// Vector + Vector = Vector
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator+(Vector<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    return lhs += rhs;
}

// Vector - Vector = Vector
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Vector<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    return lhs -= rhs;
}

// Point-related operations:

// Vector += Point (= ERROR)

// Vector + Point = Point
// template <typename Precision, std::size_t Dimension>
//[[nodiscard]] constexpr auto operator+(Vector<Precision, Dimension> const& lhs,
//                                       Point<Precision, Dimension> rhs) noexcept {
//    // Implemented in Point and here, to omit unnecessary inclusion of whole point_operations.hh
//    for (auto i{0U}; i < Dimension; ++i)
//        rhs[i] += lhs[i];
//    return rhs;
//}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Freestanding operations:
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto magnitude(Vector<Precision, Dimension> const& v) noexcept {
    Precision result{};

    for (auto i{0U}; i < Dimension; ++i)
        result += v[i] * v[i];

    return std::sqrt(result); // TODO: not constexpr
}

template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto normalize(Vector<Precision, Dimension> const& v) noexcept {
    std::remove_cvref_t<decltype(v)> result;

    /*constexpr*/ auto const mag = magnitude(v);
    for (auto i{0U}; i < Dimension; ++i)
        result[i] = v[i] / mag;
    result[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(result[Dimension] == static_cast<Precision>(1));
    return result;
}

template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto dot(Vector<Precision, Dimension> const& v1,
                                 Vector<Precision, Dimension> const& v2) noexcept {
    Precision result{};

    for (auto i{0U}; i < Dimension; ++i)
        result += v1[i] * v2[i];

    return result;
}

template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr std::enable_if_t<Dimension == 3, Vector<Precision, Dimension>>
cross(Vector<Precision, Dimension> const& v1, Vector<Precision, Dimension> const& v2) noexcept {
#if __cpp_deduction_guides >= 201907
    return Vector{v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
                  v1[0] * v2[1] - v1[1] * v2[0]};
#else
    return Matrix{v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
                  v1[0] * v2[1] - v1[1] * v2[0]};
#endif
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_VECTOR_OPERATIONS_HH_

#pragma once

#include "axis.hh"
#include "detail/types.hh"
#include "matrix.hh"
#include "point.hh"
#include "vector.hh"

#include <boost/assert.hpp>

#include <cassert>
#include <iostream>

namespace cherry_blazer {

// Matrix<NxM> * Matrix<MxP> = Matrix<N,P>
template <typename Precision, std::size_t OuterDimension, std::size_t CompatibleDimension,
          std::size_t InnerDimension>
[[nodiscard]] constexpr auto
operator*(Matrix<Precision, OuterDimension, CompatibleDimension> const& lhs,
          Matrix<Precision, CompatibleDimension, InnerDimension> const& rhs) noexcept {

    Matrix<Precision, OuterDimension, InnerDimension> result;

    for (auto row{0U}; row < OuterDimension; ++row) {
        for (auto col{0U}; col < InnerDimension; ++col) {
            result(row, col) = static_cast<Precision>(0);
            for (auto inner{0U}; inner < CompatibleDimension; ++inner)
                result(row, col) += lhs(row, inner) * rhs(inner, col);
        }
    }

    return result;
}

// Matrix * Vector = Vector
template <typename Precision, std::size_t OuterDimension, std::size_t CompatibleDimension>
[[nodiscard]] constexpr auto
operator*(Matrix<Precision, OuterDimension, CompatibleDimension> const& lhs,
          Vector<Precision, CompatibleDimension - 1> const& rhs) noexcept {

    std::remove_cvref_t<decltype(rhs)> result;

    for (auto row{0U}; row < OuterDimension; ++row) {
        result[row] = static_cast<Precision>(0);
        for (auto inner{0U}; inner < CompatibleDimension; ++inner)
            result[row] += lhs(row, inner) * rhs[inner];
    }

    BOOST_VERIFY(result[CompatibleDimension - 1] == static_cast<Precision>(1));
    return result;
}

// Matrix * Point = Point
template <typename Precision, std::size_t OuterDimension, std::size_t CompatibleDimension>
[[nodiscard]] constexpr auto
operator*(Matrix<Precision, OuterDimension, CompatibleDimension> const& lhs,
          Point<Precision, CompatibleDimension - 1> const& rhs) noexcept {

    // To minimize the amount of copies (from Point to Vector, then to Vector from Point), we
    // rewrite multiplication procedure once again. This is so far the only place where redundant
    // code is needed.

    std::remove_cvref_t<decltype(rhs)> result;

    for (auto row{0U}; row < OuterDimension; ++row) {
        result[row] = static_cast<Precision>(0);
        for (auto inner{0U}; inner < CompatibleDimension; ++inner)
            result[row] += lhs(row, inner) * rhs[inner];
    }

    BOOST_VERIFY(result[CompatibleDimension - 1] == static_cast<Precision>(1));
    return result;
}

// TODO: below

// https://en.wikipedia.org/wiki/Transpose
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
[[nodiscard]] constexpr auto
transpose(Matrix<Precision, OuterDimension, InnerDimension> const& mat) noexcept {
    Matrix<Precision, OuterDimension, InnerDimension> result;
    for (auto row{0U}; row < OuterDimension; ++row) {
        for (auto col{0U}; col < InnerDimension; ++col)
            result(col, row) = mat(row, col);
    }
    return result;
}

// Determinant for 2x2 matrix.
// https://en.wikipedia.org/wiki/Determinant
template <typename Precision, std::size_t Dimension, std::enable_if_t<Dimension == 2, bool> = true>
[[nodiscard]] constexpr auto det(Matrix<Precision, Dimension, Dimension> const& mat) noexcept {
    return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
}

// Get submatrix without certain row and column.
// https://en.wikipedia.org/wiki/Matrix_(mathematics)#Submatrix
template <typename Precision, u16 OuterDimension, u16 InnerDimension>
[[nodiscard]] constexpr auto submatrix(Matrix<Precision, OuterDimension, InnerDimension> const& mat,
                                       std::size_t const& without_row,
                                       std::size_t const& without_col) noexcept {
    static_assert(OuterDimension >= 2 && InnerDimension >= 2,
                  "Matrix must be at least 2x2 to have a submatrix.");
    Matrix<Precision, OuterDimension - 1, InnerDimension - 1> submat;
    for (auto src_row{0U}, dst_row{0U}; src_row < OuterDimension; ++src_row) {
        if (src_row == without_row)
            continue;
        for (auto src_col{0U}, dst_col{0U}; src_col < InnerDimension; ++src_col) {
            if (src_col == without_col)
                continue;
            submat(dst_row, dst_col) = mat(src_row, src_col);
            ++dst_col;
        }
        ++dst_row;
    }
    return submat;
}

// Get minor of matrix at element (row, col).
// https://en.wikipedia.org/wiki/Minor_(linear_algebra)
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
[[nodiscard]] constexpr auto minor(Matrix<Precision, OuterDimension, InnerDimension> const& mat,
                                   std::size_t const& row, std::size_t const& col) noexcept {
    return det(submatrix(mat, row, col));
}

// Get cofactor of matrix at element (row, col).
// https://en.wikipedia.org/wiki/Minor_(linear_algebra)
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
[[nodiscard]] constexpr auto cofactor(Matrix<Precision, OuterDimension, InnerDimension> const& mat,
                                      std::size_t const& row, std::size_t const& col) noexcept {
    auto result = minor(mat, row, col);
    return (row + col) % 2 == 0 ? result : -result;
}

// Determinant for NxN matrix.
// https://en.wikipedia.org/wiki/Determinant
template <typename Precision, std::size_t Dimension, std::enable_if_t<Dimension >= 3, bool> = true>
[[nodiscard]] constexpr auto det(Matrix<Precision, Dimension, Dimension> const& mat) noexcept {
    Precision determinant{};
    for (auto col{0U}; col < Dimension; ++col)
        determinant += mat(0, col) * cofactor(mat, 0, col);
    return determinant;
}

// https://en.wikipedia.org/wiki/Invertible_matrix
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto
invertible(Matrix<Precision, Dimension, Dimension> const& mat) noexcept {
    return det(mat) != 0;
}

// Find an inverse of a matrix.
// https://en.wikipedia.org/wiki/Invertible_matrix
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto inverse(Matrix<Precision, Dimension, Dimension> const& mat) {
    auto determinant = det(mat);
    if (determinant == 0)
        throw std::logic_error{"Cannot inverse matrix, because det = 0."};
    Matrix<Precision, Dimension, Dimension> inverted;
    for (auto row{0U}; row < Dimension; ++row) {
        for (auto col{0U}; col < Dimension; ++col) {
            inverted(col, row) = cofactor(mat, row, col) / determinant;
        }
    }
    return inverted;
}

} // namespace cherry_blazer

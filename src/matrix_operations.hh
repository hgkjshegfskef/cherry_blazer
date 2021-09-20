#ifndef CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

#include "matrix.hh"
#include "point.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"
#include "vector.hh"

#include <cassert>
#include <iostream>

namespace cherry_blazer {

// Matrix<NxM> * Matrix<MxP> = Matrix<N,P>
template <typename T, u16 N, u16 M, u16 P>
[[nodiscard]] constexpr auto operator*(Matrix<T, N, M> const& lhs,
                                       Matrix<T, M, P> const& rhs) noexcept {
    Matrix<T, N, P> result;
    for (auto row{0U}; row < N; ++row) {
        for (auto col{0U}; col < P; ++col) {
            result(row, col) = 0;
            for (auto inner{0U}; inner < M; ++inner)
                result(row, col) += lhs(row, inner) * rhs(inner, col);
        }
    }
    return result;
}

// Matrix * Vector = Vector
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto operator*(Matrix<T, N, M> const& lhs,
                                       Vector<T, M> const& rhs) noexcept {
    Vector<T, M> result;
    for (auto row{0U}; row < N; ++row) {
        result[row] = 0;
        for (auto col{0U}; col < M; ++col)
            result[row] += lhs(row, col) * rhs[col];
    }
    return result;
}

// https://en.wikipedia.org/wiki/Transpose
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto transpose(Matrix<T, N, M> const& mat) noexcept {
    Matrix<T, N, M> result{};
    for (auto row{0U}; row < N; ++row) {
        for (auto col{0U}; col < M; ++col)
            result(col, row) = mat(row, col);
    }
    return result;
}

// Determinant for 2x2 matrix.
// https://en.wikipedia.org/wiki/Determinant
template <typename T, u16 N, std::enable_if_t<N == 2U, bool> = true>
[[nodiscard]] constexpr auto det(Matrix<T, N, N> const& mat) noexcept {
    return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
}

// Get submatrix without certain row and column.
// https://en.wikipedia.org/wiki/Matrix_(mathematics)#Submatrix
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto submatrix(Matrix<T, N, M> const& mat,
                                       safe_urange_auto<0, N - 1> const& without_row,
                                       safe_urange_auto<0, M - 1> const& without_col) noexcept {
    static_assert(N >= 2 && M >= 2, "Matrix must be at least 2x2 to have a submatrix.");
    Matrix<T, N - 1, M - 1> submat;
    for (auto src_row{0U}, dst_row{0U}; src_row < N; ++src_row) {
        if (src_row == without_row)
            continue;
        for (auto src_col{0U}, dst_col{0U}; src_col < M; ++src_col) {
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
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto minor(Matrix<T, N, M> const& mat,
                                   safe_urange_auto<0, N - 1> const& row,
                                   safe_urange_auto<0, M - 1> const& col) noexcept {
    return det(submatrix(mat, row, col));
}

// Get cofactor of matrix at element (row, col).
// https://en.wikipedia.org/wiki/Minor_(linear_algebra)
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto cofactor(Matrix<T, N, M> const& mat,
                                      safe_urange_auto<0, N - 1> const& row,
                                      safe_urange_auto<0, M - 1> const& col) noexcept {
    auto result = minor(mat, row, col);
    return (row + col) % 2 == 0 ? result : -result;
}

// Determinant for NxN matrix.
// https://en.wikipedia.org/wiki/Determinant
template <typename T, u16 N, std::enable_if_t<N >= 3U, bool> = true>
[[nodiscard]] constexpr auto det(Matrix<T, N, N> const& mat) noexcept {
    T determinant{};
    for (auto col{0U}; col < N; ++col)
        determinant += mat(0, col) * cofactor(mat, 0, col);
    return determinant;
}

// https://en.wikipedia.org/wiki/Invertible_matrix
template <typename T, u16 N>
[[nodiscard]] constexpr auto invertible(Matrix<T, N, N> const& mat) noexcept {
    return det(mat) != 0;
}

// Find an inverse of a matrix.
// https://en.wikipedia.org/wiki/Invertible_matrix
template <typename T, u16 N> [[nodiscard]] constexpr auto inverse(Matrix<T, N, N> const& mat) {
    auto determinant = det(mat);
    if (determinant == 0)
        throw std::logic_error{"Cannot inverse matrix, because det = 0."};
    Matrix<T, N, N> inverted;
    for (auto row{0U}; row < N; ++row) {
        for (auto col{0U}; col < N; ++col) {
            inverted(col, row) = cofactor(mat, row, col) / determinant;
        }
    }
    return inverted;
}

// For translation, scaling, reflection, rotation, shearing see:
// https://en.wikipedia.org/wiki/Affine_transformation#Image_transformation

// https://en.wikipedia.org/wiki/Translation_(geometry)
template <typename T, u16 D>
[[nodiscard]] constexpr auto translate(Matrix<T, D, D> const& translation_matrix,
                                       Point<T, D - 1> const& original_point) {
    Vector<T, D> augmented_vector;
    for (auto row{0U}; row < D - 1; ++row)
        augmented_vector[row] = original_point[row];
    augmented_vector[D - 1] = static_cast<T>(1);

    auto augmented_translated_vector = translation_matrix * augmented_vector;

    Point<T, D - 1> translated_point;
    for (auto row{0U}; row < D - 1; ++row)
        translated_point[row] = augmented_translated_vector[row];

    return translated_point;
}

// https://en.wikipedia.org/wiki/Scaling_(geometry)
template <typename T, u16 D>
[[nodiscard]] constexpr auto scale(Matrix<T, D, D> const& scaling_matrix,
                                   Point<T, D - 1> const& original_point) {
    Vector<T, D> augmented_vector;
    for (auto row{0U}; row < D - 1; ++row)
        augmented_vector[row] = original_point[row];
    augmented_vector[D - 1] = static_cast<T>(1);

    auto augmented_scaled_vector = scaling_matrix * augmented_vector;

    Point<T, D - 1> scaled_point;
    for (auto row{0U}; row < D - 1; ++row)
        scaled_point[row] = augmented_scaled_vector[row];

    return scaled_point;
}

// This is identical to above, except for Vector parameter.
// https://en.wikipedia.org/wiki/Scaling_(geometry)
template <typename T, u16 D>
[[nodiscard]] constexpr auto scale(Matrix<T, D, D> const& scaling_matrix,
                                   Vector<T, D - 1> const& original_vector) {
    Vector<T, D> augmented_vector;
    for (auto row{0U}; row < D - 1; ++row)
        augmented_vector[row] = original_vector[row];
    augmented_vector[D - 1] = static_cast<T>(1);

    auto augmented_scaled_vector = scaling_matrix * augmented_vector;

    Vector<T, D - 1> scaled_vector;
    for (auto row{0U}; row < D - 1; ++row)
        scaled_vector[row] = augmented_scaled_vector[row];

    return scaled_vector;
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

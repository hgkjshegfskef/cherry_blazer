#ifndef CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

#include "matrix.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"
#include "vector.hh"

#include <iostream>

namespace cherry_blazer {

// Matrix<NxM> * Matrix<MxP> = Matrix<N,P>
template <typename T, u16 N, u16 M, u16 P>
[[nodiscard]] constexpr auto operator*(Matrix<T, N, M> const& lhs,
                                       Matrix<T, M, P> const& rhs) noexcept {
    Matrix<T, N, P> result;
    for (auto i{0U}; i < N; ++i) {
        for (auto j{0U}; j < P; ++j) {
            result(i, j) = 0;
            for (auto k{0U}; k < M; ++k)
                result(i, j) += lhs(i, k) * rhs(k, j);
        }
    }
    return result;
}

// Matrix * Vector = Vector
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto operator*(Matrix<T, N, M> const& lhs,
                                       Vector<T, M> const& rhs) noexcept {
    Vector<T, M> result;
    for (auto i{0U}; i < N; ++i) {
        result[i] = 0;
        for (auto j{0U}; j < M; ++j)
            result[i] += lhs(i, j) * rhs[j];
    }
    return result;
}

// https://en.wikipedia.org/wiki/Transpose
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto transpose(Matrix<T, N, M> const& mat) {
    Matrix<T, N, M> result{};
    for (auto i{0U}; i < N; ++i) {
        for (auto j{0U}; j < M; ++j)
            result(j, i) = mat(i, j);
    }
    return result;
}

// https://en.wikipedia.org/wiki/Determinant
template <typename T, u16 N, std::enable_if_t<N == 2, bool> = true>
[[nodiscard]] constexpr auto det(Matrix<T, N, N> const& mat) {
    return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
}

// Get submatrix without certain row and column.
template <typename T, u16 N, u16 M>
[[nodiscard]] constexpr auto submatrix(Matrix<T, N, M> const& mat,
                                       safe_urange_auto<0, N - 1> const& without_row,
                                       safe_urange_auto<0, M - 1> const& without_col) {
    static_assert(N >= 2 && M >= 2, "Matrix must be at least 2x2 to have a submatrix.");
    Matrix<T, N - 1, M - 1> result;
    for (auto src_i{0U}, dst_i{0U}; src_i < N; ++src_i) {
        if (src_i == without_row)
            continue;
        for (auto src_j{0U}, dst_j{0U}; src_j < M; ++src_j) {
            if (src_j == without_col)
                continue;
            result(dst_i, dst_j) = mat(src_i, src_j);
            ++dst_j;
        }
        ++dst_i;
    }
    return result;
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

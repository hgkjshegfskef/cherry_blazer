#ifndef CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

#include "matrix.hh"
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

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

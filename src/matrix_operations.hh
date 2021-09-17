#ifndef CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

#include "matrix.hh"
#include "types.hh"
#include "vector.hh"

#include <iostream>

namespace cherry_blazer {

// Matrix<NxM> * Matrix<MxP> = Matrix<N,P>
template <u16 N, u16 M, u16 P>
[[nodiscard]] constexpr auto operator*(Matrix<N, M> const& lhs, Matrix<M, P> const& rhs) noexcept {
    Matrix<N, P> result;
    for (auto i{0U}; i < N; ++i) {
        for (auto j{0U}; j < P; ++j) {
            double sum{};
            for (auto k{0U}; k < M; ++k) {
                sum += lhs(i, k) * rhs(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

// Matrix * Vector
//template <u16 N, u16 M>
//[[nodiscard]] constexpr auto operator*(Matrix<N, M> const& lhs, Vector<M> const& rhs) noexcept {
//    Vector<M> result;
//    for (auto i{0U}; i < N; ++i) {
//        result[i] = 0;
//        for (auto j{0U}; j < M; ++j) {
//            result[i] += lhs(i, j) * rhs[j];
//        }
//    }
//    return result;
//}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_OPERATIONS_HH_

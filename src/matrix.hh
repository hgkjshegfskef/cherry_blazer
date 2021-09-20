#ifndef CHERRY_BLAZER_SRC_MATRIX_HH_
#define CHERRY_BLAZER_SRC_MATRIX_HH_

#include "safe_numerics_typedefs.hh"
#include "types.hh"
#include "util.hh"
#include "vector.hh"

#include <algorithm>
#include <array>
#include <cstddef>
#include <iomanip>
#include <memory>
#include <type_traits>
#include <utility>

namespace cherry_blazer {

namespace matrix::detail {
template <std::size_t, typename T> using indexed_type = T;
} // namespace matrix::detail

template <typename T, typename Ns, u16 M> class MatrixImpl;

template <typename T, std::size_t... Ns, u16 M> class MatrixImpl<T, std::index_sequence<Ns...>, M> {
    static inline constexpr u32 size = sizeof...(Ns) * M;

  public:
    MatrixImpl() = default;
    constexpr explicit MatrixImpl(
        matrix::detail::indexed_type<Ns, T const (&)[M]>... arr) noexcept {
        ((insert(mat_, arr, Ns)), ...);
    }

  protected:
    std::array<T, size> mat_; // NOLINT(readability-identifier-naming)

  private:
    constexpr void insert(std::array<T, size>& dst, T const (&src)[M], u16 n) noexcept {
        for (auto i{n * M}, j{0UL}; i < n * M + M; ++i, ++j)
            dst[i] = src[j];
    }
};

// Any NxM Matrix, when size is known at compile time.
template <typename T, u16 N, u16 M> class Matrix : MatrixImpl<T, std::make_index_sequence<N>, M> {
    static_assert(std::is_floating_point_v<T>);

    static inline constexpr u32 size = N * M;
    static_assert(size != 0, "Both dimensions must be non-zero.");

    using underlying_type = std::array<T, size>;
    using impl = MatrixImpl<T, std::make_index_sequence<N>, M>;

  public:
    using iterator = typename underlying_type::iterator;
    using const_iterator = typename underlying_type::const_iterator;
    using value_type = T;
    static inline constexpr u16 row_size = N;
    static inline constexpr u16 col_size = M;

    // Clang requires ctor to not be aliased: https://bugs.llvm.org/show_bug.cgi?id=22242
    // This became a subject to a defect report: https://wg21.link/cwg2070
    using MatrixImpl<T, std::make_index_sequence<N>, M>::MatrixImpl;

    [[nodiscard]] iterator begin() { return impl::mat_.begin(); }
    [[nodiscard]] iterator end() { return impl::mat_.end(); }
    [[nodiscard]] const_iterator begin() const { return impl::mat_.begin(); }
    [[nodiscard]] const_iterator end() const { return impl::mat_.end(); }
    [[nodiscard]] const_iterator cbegin() const { return impl::mat_.begin(); }
    [[nodiscard]] const_iterator cend() const { return impl::mat_.end(); }

    constexpr T& operator()(safe_urange_auto<0, N> n, safe_urange_auto<0, M> m) noexcept {
        return impl::mat_[n * M + m];
    }

    constexpr T operator()(safe_urange_auto<0, N> n, safe_urange_auto<0, M> m) const noexcept {
        return impl::mat_[n * M + m];
    }

    friend constexpr bool operator==(Matrix const& lhs, Matrix const& rhs) noexcept {
        return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
                          [](auto& lhs, auto& rhs) { return almost_equal(lhs, rhs); });
    }

    friend constexpr bool operator!=(Matrix const& lhs, Matrix const& rhs) noexcept {
        return !(lhs == rhs);
    }

    friend constexpr std::ostream& operator<<(std::ostream& os, Matrix const& mat) noexcept {
        for (auto row{0U}; row < N - 1; ++row) {
            os << "{ ";
            for (auto col{0U}; col < M - 1; ++col) {
                os << std::setprecision(std::numeric_limits<T>::max_digits10) << mat(row, col)
                   << ", ";
            }
            os << std::setprecision(std::numeric_limits<T>::max_digits10) << mat(row, M - 1)
               << " }\n";
        }

        os << "{ ";
        for (auto col{0U}; col < M - 1; ++col) {
            os << std::setprecision(std::numeric_limits<T>::max_digits10) << mat(N - 1, col)
               << ", ";
        }
        return os << std::setprecision(std::numeric_limits<T>::max_digits10) << mat(N - 1, M - 1)
                  << " }";
    }

    // https://en.wikipedia.org/wiki/Identity_matrix
    [[nodiscard]] static constexpr auto identity() {
        static_assert(N == M, "Only for square matrices.");
        Matrix<T, N, N> identity_matrix{};
        for (auto i{0U}; i < N; ++i)
            identity_matrix(i, i) = static_cast<T>(1);
        return identity_matrix;
    }

    // https://en.wikipedia.org/wiki/Translation_(geometry)
    [[nodiscard]] static constexpr auto translation(Vector<T, N - 1> const& translation_vector) {
        static_assert(N == M, "Only for square matrices.");
        Matrix<T, N, N> translation_matrix = identity();
        for (auto row{0U}; row < N - 1; ++row)
            translation_matrix(row, N - 1) = translation_vector[row];
        return translation_matrix;
    }

    // https://en.wikipedia.org/wiki/Scaling_(geometry)
    [[nodiscard]] static constexpr auto scaling(Vector<T, N - 1> const& scaling_vector) {
        static_assert(N == M, "Only for square matrices.");
        Matrix<T, N, N> translation_matrix = identity();
        for (auto row{0U}; row < N - 1; ++row)
            translation_matrix(row, row) = scaling_vector[row];
        return translation_matrix;
    }
};

// Deduct a few commonly used matrices. I don't know if it's possible to use variadic pack to make a
// single guide, or at least deduct that T should be std::common_type of all Ts in the pack.

template <typename T, u16 M>
Matrix(matrix::detail::indexed_type<0U, T const (&)[M]>,
       matrix::detail::indexed_type<1U, T const (&)[M]>) -> Matrix<T, M, M>;

template <typename T, u16 M>
Matrix(matrix::detail::indexed_type<0U, T const (&)[M]>,
       matrix::detail::indexed_type<1U, T const (&)[M]>,
       matrix::detail::indexed_type<2U, T const (&)[M]>) -> Matrix<T, M, M>;

template <typename T, u16 M>
Matrix(matrix::detail::indexed_type<0U, T const (&)[M]>,
       matrix::detail::indexed_type<1U, T const (&)[M]>,
       matrix::detail::indexed_type<2U, T const (&)[M]>,
       matrix::detail::indexed_type<3U, T const (&)[M]>) -> Matrix<T, M, M>;

using Mat2f = Matrix<float, 2, 2>;  // NOLINT(readability-identifier-naming)
using Mat3f = Matrix<float, 3, 3>;  // NOLINT(readability-identifier-naming)
using Mat4f = Matrix<float, 4, 4>;  // NOLINT(readability-identifier-naming)
using Mat2d = Matrix<double, 2, 2>; // NOLINT(readability-identifier-naming)
using Mat3d = Matrix<double, 3, 3>; // NOLINT(readability-identifier-naming)
using Mat4d = Matrix<double, 4, 4>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_HH_

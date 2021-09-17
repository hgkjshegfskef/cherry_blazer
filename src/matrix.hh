#ifndef CHERRY_BLAZER_SRC_MATRIX_HH_
#define CHERRY_BLAZER_SRC_MATRIX_HH_

#include "safe_numerics_typedefs.hh"
#include "types.hh"
#include "util.hh"

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace cherry_blazer {

namespace matrix::detail {
template <std::size_t, typename T> using pair = T;
} // namespace matrix::detail

template <typename T, typename Ns, u16 M> class MatrixImpl;

template <typename T, std::size_t... Ns, u16 M> class MatrixImpl<T, std::index_sequence<Ns...>, M> {
    static inline constexpr u32 size = sizeof...(Ns) * M;

  public:
    MatrixImpl() = default;
    constexpr explicit MatrixImpl(matrix::detail::pair<Ns, T const (&)[M]>... arr) noexcept {
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

    using value_type = std::array<T, size>;
    using impl = MatrixImpl<T, std::make_index_sequence<N>, M>;

  public:
    using iterator = typename value_type::iterator;
    using const_iterator = typename value_type::const_iterator;

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
        os << "{";
        std::copy(std::cbegin(mat), std::prev(std::cend(mat)), std::ostream_iterator<T>{os, ", "});
        return os << *std::prev(std::cend(mat)) << "}";
    }
};

// Deduct a few commonly used matrices. I don't know if it's possible to use variadic pack to make a
// single guide, or at least deduct that T should be std::common_type of all Ts in the pack.

template <typename T, u16 M>
Matrix(matrix::detail::pair<0U, T const (&)[M]>, matrix::detail::pair<1U, T const (&)[M]>)
    -> Matrix<T, M, M>;

template <typename T, u16 M>
Matrix(matrix::detail::pair<0U, T const (&)[M]>, matrix::detail::pair<1U, T const (&)[M]>,
       matrix::detail::pair<2U, T const (&)[M]>) -> Matrix<T, M, M>;

template <typename T, u16 M>
Matrix(matrix::detail::pair<0U, T const (&)[M]>, matrix::detail::pair<1U, T const (&)[M]>,
       matrix::detail::pair<2U, T const (&)[M]>, matrix::detail::pair<3U, T const (&)[M]>)
    -> Matrix<T, M, M>;

using Mat2f = Matrix<float, 2, 2>;
using Mat3f = Matrix<float, 3, 3>;
using Mat4f = Matrix<float, 4, 4>;
using Mat2d = Matrix<double, 2, 2>;
using Mat3d = Matrix<double, 3, 3>;
using Mat4d = Matrix<double, 4, 4>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_HH_

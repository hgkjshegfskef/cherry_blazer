#ifndef CHERRY_BLAZER_SRC_MATRIX_HH_
#define CHERRY_BLAZER_SRC_MATRIX_HH_
#include "matrix_detail.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"
#include "util.hh"

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <utility>

namespace cherry_blazer {

namespace matrix::detail {
template <std::size_t, typename T> using pair = T;
} // namespace matrix::detail

template <typename Ns, u16 M> class MatrixImpl;

template <std::size_t... Ns, u16 M> class MatrixImpl<std::index_sequence<Ns...>, M> {
    static inline constexpr u32 size = sizeof...(Ns) * M;

  public:
    MatrixImpl() = default;
    constexpr explicit MatrixImpl(matrix::detail::pair<Ns, double const (&)[M]>... arr) noexcept {
        ((insert(mat_, arr, Ns)), ...);
    }

  protected:
    std::array<double, size> mat_; // NOLINT(readability-identifier-naming)

  private:
    constexpr void insert(std::array<double, size>& dst, double const (&src)[M], u16 n) noexcept {
        for (auto i{n * M}, j{0UL}; i < n * M + M; ++i, ++j)
            dst[i] = src[j];
    }
};

// Any NxM Matrix, when size is known at compile time.
template <u16 N, u16 M> class Matrix : MatrixImpl<std::make_index_sequence<N>, M> {
    static inline constexpr u32 size = N * M;
    static_assert(size != 0, "Both dimensions must be non-zero.");

    using value_type = std::array<double, size>;
    using impl = MatrixImpl<std::make_index_sequence<N>, M>;

  public:
    using iterator = typename value_type::iterator;
    using const_iterator = typename value_type::const_iterator;

    // Clang requires ctor to not be aliased: https://bugs.llvm.org/show_bug.cgi?id=22242
    // This became a subject to a defect report: https://wg21.link/cwg2070
    using MatrixImpl<std::make_index_sequence<N>, M>::MatrixImpl;

    [[nodiscard]] iterator begin() { return impl::mat_.begin(); }
    [[nodiscard]] iterator end() { return impl::mat_.end(); }
    [[nodiscard]] const_iterator begin() const { return impl::mat_.begin(); }
    [[nodiscard]] const_iterator end() const { return impl::mat_.end(); }
    [[nodiscard]] const_iterator cbegin() const { return impl::mat_.begin(); }
    [[nodiscard]] const_iterator cend() const { return impl::mat_.end(); }

    constexpr double& operator()(safe_urange_auto<0, N> n, safe_urange_auto<0, M> m) noexcept {
        return impl::mat_[n * M + m];
    }

    constexpr double operator()(safe_urange_auto<0, N> n, safe_urange_auto<0, M> m) const noexcept {
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
        std::copy(std::cbegin(mat), std::prev(std::cend(mat)),
                  std::ostream_iterator<double>{os, ", "});
        return os << *std::prev(std::cend(mat)) << "}";
    }
};

// Deduct a few commonly used matrices.

Matrix(matrix::detail::pair<0UL, double const (&)[2]>,
       matrix::detail::pair<1UL, double const (&)[2]>) -> Matrix<2, 2>;

Matrix(matrix::detail::pair<0UL, double const (&)[3]>,
       matrix::detail::pair<1UL, double const (&)[3]>,
       matrix::detail::pair<2UL, double const (&)[3]>) -> Matrix<3, 3>;

Matrix(matrix::detail::pair<0UL, double const (&)[4]>,
       matrix::detail::pair<1UL, double const (&)[4]>,
       matrix::detail::pair<2UL, double const (&)[4]>,
       matrix::detail::pair<3UL, double const (&)[4]>) -> Matrix<4, 4>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_HH_

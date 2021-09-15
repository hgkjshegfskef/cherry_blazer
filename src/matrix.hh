#ifndef CHERRY_BLAZER_SRC_MATRIX_HH_
#define CHERRY_BLAZER_SRC_MATRIX_HH_

#include "matrix_detail.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <utility>

namespace cherry_blazer {

template <typename Ns, u16 M> struct MatrixImpl;

template <std::size_t... Ns, u16 M> struct MatrixImpl<std::index_sequence<Ns...>, M> {
    static inline constexpr u32 size = sizeof...(Ns) * M;

    template <std::size_t, typename T> using pair = T;

  public:
    constexpr explicit MatrixImpl(pair<Ns, double const (&)[M]>... arr) noexcept {
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
template <u16 N, u16 M> class Matrix : public MatrixImpl<std::make_index_sequence<N>, M> {
    static inline constexpr u32 size = N * M;
    static_assert(size != 0, "Both dimensions must be non-zero.");

    using value_type = std::array<double, size>;

  public:
    using iterator = typename value_type::iterator;
    using const_iterator = typename value_type::const_iterator;
    using impl = MatrixImpl<std::make_index_sequence<N>, M>;

    // Clang requires ctor to not be aliased: https://bugs.llvm.org/show_bug.cgi?id=22242
    // This became a subject to a defect report: https://wg21.link/cwg2070
    using MatrixImpl<std::make_index_sequence<N>, M>::MatrixImpl;

    [[nodiscard]] iterator begin() { return impl::mat_.begin(); }
    [[nodiscard]] iterator end() { return impl::mat_.end(); }
    [[nodiscard]] const_iterator begin() const { return impl::mat_.begin(); }
    [[nodiscard]] const_iterator end() const { return impl::mat_.end(); }
    [[nodiscard]] const_iterator cbegin() const { return impl::mat_.begin(); }
    [[nodiscard]] const_iterator cend() const { return impl::mat_.end(); }
};

// Any NxM Matrix, when size is known at runtime.
template <> class Matrix<0, 0> {
  public:
    using iterator = matrix::detail::Iterator<double>;
    using const_iterator = matrix::detail::ConstantIterator<double>;

    Matrix(safe_auto<u16> const& n, safe_auto<u16> const& m)
        : n_{n}, m_{m}, mat_{new double[u32(n * m)]()} {}

    [[nodiscard]] iterator begin() { return iterator{mat_.get()}; }
    [[nodiscard]] iterator end() { return iterator{mat_.get() + u32(size())}; }
    [[nodiscard]] const_iterator begin() const { return const_iterator{mat_.get()}; }
    [[nodiscard]] const_iterator end() const { return const_iterator{mat_.get() + u32(size())}; }
    [[nodiscard]] const_iterator cbegin() const { return const_iterator{mat_.get()}; }
    [[nodiscard]] const_iterator cend() const { return const_iterator{mat_.get() + u32(size())}; }

  private:
    safe_auto<u16> n_;
    safe_auto<u16> m_;
    std::unique_ptr<double[]> mat_;

    [[nodiscard]] constexpr safe_auto<u32> size() const { return n_ * m_; }
};

// Deduction guide to hide that Matrix<0,0> is used as special case.
Matrix(size_t n, size_t m) -> Matrix<0, 0>;

template <size_t N, size_t M>
constexpr std::ostream& operator<<(std::ostream& os, Matrix<N, M> const& mat) noexcept {
    os << "{";
    std::copy(std::cbegin(mat), std::prev(std::cend(mat)), std::ostream_iterator<double>{os, ", "});
    return os << *std::prev(std::cend(mat)) << "}";
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_MATRIX_HH_
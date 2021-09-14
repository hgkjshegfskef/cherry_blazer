#ifndef CHERRY_BLAZER_SRC_MATRIX_HH_
#define CHERRY_BLAZER_SRC_MATRIX_HH_

#include "matrix_detail.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>

namespace cherry_blazer {

// Any NxM Matrix, when size is known at compile time.
template <u32 N, u32 M> class Matrix {
    static inline constexpr u64 size = N * M;
    static_assert(size != 0, "Both dimensions must be non-zero.");

    using value_type = std::array<double, size>;

  public:
    using iterator = typename value_type::iterator;
    using const_iterator = typename value_type::const_iterator;

    constexpr explicit Matrix(std::array<std::array<double, M>, N> const& std_arr2d) noexcept {
        for (auto n{0U}; n < N; ++n) {
            for (auto m{0U}; m < M; ++m)
                mat_[n * M + m] = std_arr2d[n][m];
        }
    }

    constexpr explicit Matrix(double const (&arr2d)[N][M]) noexcept {
        // This makes GCC crash, lol: https://godbolt.org/z/E63r3G6o9
        // std::copy(&arr[0][0], &arr[0][0] + size, std::begin(mat_));
        for (auto n{0U}; n < N; ++n) {
            for (auto m{0U}; m < M; ++m)
                mat_[n * M + m] = arr2d[n][m];
        }
    }

    constexpr explicit Matrix(double const (&arr)[size]) noexcept : mat_{std::to_array(arr)} {}

    template <typename... Args> constexpr explicit Matrix(Args&&... args) noexcept : mat_{args...} {
        static_assert(sizeof...(args) == size, "Amount of arguments doesn't correspond to matrix "
                                               "size requested in template arguments.");
    }

    [[nodiscard]] iterator begin() { return mat_.begin(); }
    [[nodiscard]] iterator end() { return mat_.end(); }
    [[nodiscard]] const_iterator begin() const { return mat_.begin(); }
    [[nodiscard]] const_iterator end() const { return mat_.end(); }
    [[nodiscard]] const_iterator cbegin() const { return mat_.begin(); }
    [[nodiscard]] const_iterator cend() const { return mat_.end(); }

  private:
    std::array<double, size> mat_;
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

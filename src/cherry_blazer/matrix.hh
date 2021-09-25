#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_MATRIX_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_MATRIX_HH_

#include "axis.hh"
#include "shearing.hh"
#include "types.hh"
#include "util.hh"
#include "vector.hh"

#include <boost/assert.hpp>
#include <boost/io/ios_state.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <memory>
#include <type_traits>
#include <utility>

namespace cherry_blazer {

namespace detail {

template <std::size_t, typename T> using enumerate = T;

// Problem: need to be able to pass variadic pack of variadic packs of arguments with compile-time
// known length of every pack to Matrix ctor.
// Solution: this class.
// Special thanks to: wreien#0241, who wrote the first implementation with index_sequence.
template <typename Precision, typename NthInnerArrayIndexSequence, std::size_t InnerDimension>
class MatrixImpl;
template <typename Precision, std::size_t... NthInnerArrayPack, std::size_t InnerDimension>
class MatrixImpl<Precision, std::index_sequence<NthInnerArrayPack...>, InnerDimension> {
  public:
    MatrixImpl() = default;
    constexpr explicit MatrixImpl(
        enumerate<NthInnerArrayPack,
                  Precision const (&)[InnerDimension]>... nth_inner_array) noexcept {
        // memcpy() is more efficient, but it is not constexpr.
        if (std::is_constant_evaluated()) {
            ((insert(mat_, nth_inner_array, NthInnerArrayPack)), ...);
        } else {
            ((std::memcpy(mat_.data() + NthInnerArrayPack * InnerDimension, nth_inner_array,
                          sizeof nth_inner_array)),
             ...);
        }
    }

  protected:
    std::array<Precision, sizeof...(NthInnerArrayPack) * InnerDimension>
        mat_; // NOLINT(readability-identifier-naming)

  public:
    // workaround: not possible to get mat_ type in child classes, because it is protected and
    // therefore not available in any context other than non-static member functions of the derived
    // class. Thus, it is necessary to type alias it here, then it is re-aliased in child class.
    using underlying_type = decltype(mat_);

  private:
    constexpr void insert(underlying_type& dst, Precision const (&src)[InnerDimension],
                          std::size_t nth_pack) noexcept {
        for (auto i{nth_pack * InnerDimension}, j{0UL};
             i < nth_pack * InnerDimension + InnerDimension; ++i, ++j)
            dst[i] = src[j];
    }
};

// Base class with routines for any NxM Matrix.
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class MatrixBase
    : public MatrixImpl<Precision, std::make_index_sequence<OuterDimension>, InnerDimension> {

    static_assert(std::is_same_v<Precision, float> || std::is_same_v<Precision, double>,
                  "Matrix only supports single and double precision.");

    static_assert(OuterDimension * InnerDimension != 0, "Both dimensions must be non-zero.");

    using impl = MatrixImpl<Precision, std::make_index_sequence<OuterDimension>, InnerDimension>;
    using underlying_type = typename impl::underlying_type;

  public:
    static inline constexpr std::size_t outer_dimension = OuterDimension;
    static inline constexpr std::size_t inner_dimension = InnerDimension;

    MatrixBase() = default;
    using MatrixImpl<Precision, std::make_index_sequence<OuterDimension>,
                     InnerDimension>::MatrixImpl;

    // Reuse std::array interface.

    using value_type = Precision;
    using size_type = typename underlying_type::size_type;
    using difference_type = typename underlying_type::difference_type;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = typename underlying_type::iterator;
    using const_iterator = typename underlying_type::const_iterator;
    using reverse_iterator = typename underlying_type::reverse_iterator;
    using const_reverse_iterator = typename underlying_type::const_reverse_iterator;

    constexpr reference at(size_type outer_pos, size_type inner_pos) {
        return impl::mat_.at(outer_pos * InnerDimension + inner_pos);
    }
    [[nodiscard]] constexpr const_reference at(size_type outer_pos, size_type inner_pos) const {
        return impl::mat_.at(outer_pos * InnerDimension + inner_pos);
    }

    constexpr reference operator()(size_type outer_pos, size_type inner_pos) noexcept {
        BOOST_VERIFY(outer_pos < OuterDimension);
        BOOST_VERIFY(inner_pos < InnerDimension);
        return impl::mat_[outer_pos * InnerDimension + inner_pos];
    }
    constexpr const_reference operator()(size_type outer_pos, size_type inner_pos) const noexcept {
        BOOST_VERIFY(outer_pos < OuterDimension);
        BOOST_VERIFY(inner_pos < InnerDimension);
        return impl::mat_[outer_pos * InnerDimension + inner_pos];
    }

    constexpr reference front() { return impl::mat_.front(); }
    [[nodiscard]] constexpr const_reference front() const { return impl::mat_.front(); }

    constexpr reference back() { return impl::mat_.back(); }
    [[nodiscard]] constexpr const_reference back() const { return impl::mat_.back(); }

    constexpr pointer data() noexcept { return impl::mat_.data(); }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return impl::mat_.data(); }

    constexpr iterator begin() noexcept { return impl::mat_.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return impl::mat_.begin(); }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return impl::mat_.cbegin(); }

    constexpr iterator end() noexcept { return impl::mat_.end(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return impl::mat_.end(); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return impl::mat_.cend(); }

    constexpr reverse_iterator rbegin() noexcept { return impl::mat_.rbegin(); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
        return impl::mat_.rbegin();
    }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
        return impl::mat_.crbegin();
    }

    constexpr reverse_iterator rend() noexcept { return impl::mat_.rend(); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
        return impl::mat_.rend();
    }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
        return impl::mat_.crend();
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return impl::mat_.empty(); }

    [[nodiscard]] constexpr size_type size() const noexcept { return impl::mat_.size(); }

    [[nodiscard]] constexpr size_type max_size() const noexcept { return impl::mat_.max_size(); }

    constexpr void fill(const_reference value) { return impl::mat_.fill(value); }

    constexpr void swap(MatrixBase& other) noexcept(std::is_nothrow_swappable_v<value_type>) {
        using std::swap;
        impl::mat_.swap(other.impl::mat_);
    }
};

} // namespace detail

// Any NxM matrix. Reuse base class routines.
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class Matrix : public detail::MatrixBase<Precision, OuterDimension, InnerDimension> {
  public:
    Matrix() = default;
    using detail::MatrixBase<Precision, OuterDimension, InnerDimension>::MatrixBase;
};

// Square matrix. Reuse base class routines, and add new ones.
template <typename Precision, std::size_t Dimension>
class Matrix<Precision, Dimension, Dimension>
    : public detail::MatrixBase<Precision, Dimension, Dimension> {
  public:
    Matrix() = default;
    using detail::MatrixBase<Precision, Dimension, Dimension>::MatrixBase;

    // https://en.wikipedia.org/wiki/Identity_matrix
    [[nodiscard]] static constexpr auto identity() {
        Matrix<Precision, Dimension, Dimension> identity_matrix{};
        for (auto i{0U}; i < Dimension; ++i)
            identity_matrix(i, i) = static_cast<Precision>(1);
        return identity_matrix;
    }

    // https://en.wikipedia.org/wiki/Translation_(geometry)
    [[nodiscard]] static constexpr auto
    translation(Vector<Precision, Dimension - 1> const& translation_vector) {
        auto translation_matrix = identity();
        for (auto row{0U}; row < Dimension - 1; ++row)
            translation_matrix(row, Dimension - 1) = translation_vector[row];
        return translation_matrix;
    }

    // https://en.wikipedia.org/wiki/Scaling_(geometry)
    [[nodiscard]] static constexpr auto
    scaling(Vector<Precision, Dimension - 1> const& scaling_vector) {
        auto scaling_matrix = identity();
        for (auto row{0U}; row < Dimension - 1; ++row)
            scaling_matrix(row, row) = scaling_vector[row];
        return scaling_matrix;
    }

    // https://en.wikipedia.org/wiki/Rotation_matrix
    // Rotation around X, Y, or Z axis.
    [[nodiscard]] static constexpr auto rotation(Axis const& axis, Precision const& radians) {
        static_assert(Dimension == 4, "Only for 3D.");
        auto const sine = std::sin(radians);
        auto const cosine = std::cos(radians);
        auto rotation_matrix = identity();
        switch (axis) {
        case Axis::X:
            rotation_matrix(1, 1) = cosine;
            rotation_matrix(1, 2) = -sine;
            rotation_matrix(2, 1) = sine;
            rotation_matrix(2, 2) = cosine;
            return rotation_matrix;
        case Axis::Y:
            rotation_matrix(0, 0) = cosine;
            rotation_matrix(0, 2) = sine;
            rotation_matrix(2, 0) = -sine;
            rotation_matrix(2, 2) = cosine;
            return rotation_matrix;
        case Axis::Z:
            rotation_matrix(0, 0) = cosine;
            rotation_matrix(0, 1) = -sine;
            rotation_matrix(1, 0) = sine;
            rotation_matrix(1, 1) = cosine;
            return rotation_matrix;
        }
        unreachable("Axis has only X,Y,Z");
    }

    [[nodiscard]] static constexpr auto shearing(ShearDirection const& direction) {
        static_assert(Dimension == 4, "Only for 3D.");
        auto shearing_matrix = identity();
        std::visit(
            [&](auto&& kind) {
                using shear_kind = std::decay_t<decltype(kind)>;
                if constexpr (std::is_same_v<shear_kind, Shear::X::AgainstY>) {
                    shearing_matrix(0, 1) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::X::AgainstZ>) {
                    shearing_matrix(0, 2) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Y::AgainstX>) {
                    shearing_matrix(1, 0) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Y::AgainstZ>) {
                    shearing_matrix(1, 2) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Z::AgainstX>) {
                    shearing_matrix(2, 0) = static_cast<Precision>(1);
                } else if constexpr (std::is_same_v<shear_kind, Shear::Z::AgainstY>) {
                    shearing_matrix(2, 1) = static_cast<Precision>(1);
                } else {
                    static_assert(always_false_v<shear_kind>, "non-exhaustive visitor");
                }
            },
            direction);
        return shearing_matrix;
    }
};

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
constexpr bool operator==(Matrix<Precision, OuterDimension, InnerDimension> const& lhs,
                          Matrix<Precision, OuterDimension, InnerDimension> const& rhs) noexcept {
    // TODO: almost_equal is not constexpr
    return std::equal(std::cbegin(lhs), std::cend(lhs), std::cbegin(rhs), std::cend(rhs),
                      [](auto& lhs, auto& rhs) { return almost_equal(lhs, rhs); });
}

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
constexpr bool operator!=(Matrix<Precision, OuterDimension, InnerDimension> const& lhs,
                          Matrix<Precision, OuterDimension, InnerDimension> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
std::ostream& operator<<(std::ostream& os,
                         Matrix<Precision, OuterDimension, InnerDimension> const& mat) noexcept {
    boost::io::ios_precision_saver ips{os};
    os.precision(
        std::numeric_limits<
            typename Matrix<Precision, OuterDimension, InnerDimension>::value_type>::max_digits10);

    for (auto row{0U}; row < OuterDimension - 1; ++row) {
        for (auto col{0U}; col < InnerDimension - 1; ++col) {
            os << mat(row, col) << '\t';
        }
        os << mat(row, InnerDimension - 1) << '\n';
    }

    for (auto col{0U}; col < InnerDimension - 1; ++col) {
        os << mat(OuterDimension - 1, col) << '\t';
    }
    os << mat(OuterDimension - 1, InnerDimension - 1);

    ips.restore();

    return os;
}

template <std::size_t Ith, std::size_t Jth, typename Precision, std::size_t OuterDimension,
          std::size_t InnerDimension>
constexpr typename Matrix<Precision, OuterDimension, InnerDimension>::reference
get(Matrix<Precision, OuterDimension, InnerDimension>& mat) noexcept {
    static_assert(0 <= Ith && Ith < OuterDimension, "Matrix outer index is out of bounds.");
    static_assert(0 <= Jth && Jth < InnerDimension, "Matrix inner index is out of bounds.");
    return mat(Ith, Jth);
}
template <std::size_t Ith, std::size_t Jth, typename Precision, std::size_t OuterDimension,
          std::size_t InnerDimension>
constexpr typename Matrix<Precision, OuterDimension, InnerDimension>::const_reference
get(Matrix<Precision, OuterDimension, InnerDimension> const& mat) noexcept {
    static_assert(0 <= Ith && Ith < OuterDimension, "Matrix outer index is out of bounds.");
    static_assert(0 <= Jth && Jth < InnerDimension, "Matrix inner index is out of bounds.");
    return mat(Ith, Jth);
}

// Manually deduce a few commonly used matrices (if you know a better way, contact me!).

// 1xM Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, 1, InnerDimension>;

// 2x2 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

// 3x3 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>,
       detail::enumerate<2, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

// 4x4 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0, Precision const (&)[InnerDimension]>,
       detail::enumerate<1, Precision const (&)[InnerDimension]>,
       detail::enumerate<2, Precision const (&)[InnerDimension]>,
       detail::enumerate<3, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

using Mat2f = Matrix<float, 2, 2>;  // NOLINT(readability-identifier-naming)
using Mat3f = Matrix<float, 3, 3>;  // NOLINT(readability-identifier-naming)
using Mat4f = Matrix<float, 4, 4>;  // NOLINT(readability-identifier-naming)
using Mat2d = Matrix<double, 2, 2>; // NOLINT(readability-identifier-naming)
using Mat3d = Matrix<double, 3, 3>; // NOLINT(readability-identifier-naming)
using Mat4d = Matrix<double, 4, 4>; // NOLINT(readability-identifier-naming)

// Explicitly instantiate commonly used matrices.
extern template class Matrix<double, 4, 4>;
extern template class Matrix<double, 1, 4>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_MATRIX_HH_

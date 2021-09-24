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

template <typename Precision, typename NthInnerArrayIndexSequence, std::size_t InnerDimension>
class MatrixBase;

template <typename Precision, std::size_t... NthInnerArrayPack, std::size_t InnerDimension>
class MatrixBase<Precision, std::index_sequence<NthInnerArrayPack...>, InnerDimension> {
  public:
    MatrixBase() = default;
    constexpr explicit MatrixBase(
        enumerate<NthInnerArrayPack,
                  Precision const (&)[InnerDimension]>... nth_inner_array) noexcept {
        ((std::memcpy(mat_.data() + NthInnerArrayPack * InnerDimension, nth_inner_array,
                      sizeof nth_inner_array)),
         ...);
    }

  protected:
    std::array<Precision, sizeof...(NthInnerArrayPack) * InnerDimension>
        mat_; // NOLINT(readability-identifier-naming)
};

} // namespace detail

// Any NxM Matrix, when size is known at compile time.
template <typename Precision, std::size_t OuterDimension, std::size_t InnerDimension>
class Matrix
    : detail::MatrixBase<Precision, std::make_index_sequence<OuterDimension>, InnerDimension> {
    static_assert(std::is_same_v<Precision, float> || std::is_same_v<Precision, double>,
                  "Matrix only supports single and double precision.");

    static_assert(OuterDimension * InnerDimension != 0, "Both dimensions must be non-zero.");

    using underlying_type = std::array<Precision, OuterDimension * InnerDimension>;
    using base =
        detail::MatrixBase<Precision, std::make_index_sequence<OuterDimension>, InnerDimension>;

  public:
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

    static inline constexpr std::size_t outer_dimension = OuterDimension;
    static inline constexpr std::size_t inner_dimension = InnerDimension;

    Matrix() = default;
    using detail::MatrixBase<Precision, std::make_index_sequence<OuterDimension>,
                             InnerDimension>::MatrixBase;

    // Implement interface similar to std::array.

    constexpr reference at(size_type outer_pos, size_type inner_pos) {
        return base::mat_.at(outer_pos * InnerDimension + inner_pos);
    }
    constexpr const_reference at(size_type outer_pos, size_type inner_pos) const {
        return base::mat_.at(outer_pos * InnerDimension + inner_pos);
    }

    constexpr reference operator()(size_type outer_pos, size_type inner_pos) noexcept {
        BOOST_VERIFY(outer_pos < OuterDimension);
        BOOST_VERIFY(inner_pos < InnerDimension);
        return base::mat_[outer_pos * InnerDimension + inner_pos];
    }
    constexpr const_reference operator()(size_type outer_pos, size_type inner_pos) const noexcept {
        BOOST_VERIFY(outer_pos < OuterDimension);
        BOOST_VERIFY(inner_pos < InnerDimension);
        return base::mat_[outer_pos * InnerDimension + inner_pos];
    }

    constexpr reference front() { return base::mat_.front(); }
    constexpr const_reference front() const { return base::mat_.front(); }

    constexpr reference back() { return base::mat_.back(); }
    constexpr const_reference back() const { return base::mat_.back(); }

    constexpr pointer data() noexcept { return base::mat_.data(); }
    constexpr const_pointer data() const noexcept { return base::mat_.data(); }

    constexpr iterator begin() noexcept { return base::mat_.begin(); }
    constexpr const_iterator begin() const noexcept { return base::mat_.begin(); }
    constexpr const_iterator cbegin() const noexcept { return base::mat_.cbegin(); }

    constexpr iterator end() noexcept { return base::mat_.end(); }
    constexpr const_iterator end() const noexcept { return base::mat_.end(); }
    constexpr const_iterator cend() const noexcept { return base::mat_.cend(); }

    constexpr reverse_iterator rbegin() noexcept { return base::mat_.rbegin(); }
    constexpr const_reverse_iterator rbegin() const noexcept { return base::mat_.rbegin(); }
    constexpr const_reverse_iterator crbegin() const noexcept { return base::mat_.crbegin(); }

    constexpr reverse_iterator rend() noexcept { return base::mat_.rend(); }
    constexpr const_reverse_iterator rend() const noexcept { return base::mat_.rend(); }
    constexpr const_reverse_iterator crend() const noexcept { return base::mat_.crend(); }

    [[nodiscard]] constexpr bool empty() const noexcept { return base::mat_.empty(); }

    constexpr size_type size() const noexcept { return base::mat_.size(); }

    constexpr size_type max_size() const noexcept { return base::mat_.max_size(); }

    constexpr void fill(const_reference value) { return base::mat_.fill(value); }

    constexpr void swap(Matrix& other) noexcept(std::is_nothrow_swappable_v<value_type>) {
        using std::swap;
        base::mat_.swap(other.base::mat_);
    }

    // https://en.wikipedia.org/wiki/Identity_matrix
    [[nodiscard]] static constexpr auto identity() {
        static_assert(OuterDimension == InnerDimension, "Only for square matrices.");
        Matrix<Precision, OuterDimension, OuterDimension> identity_matrix{};
        for (auto i{0U}; i < OuterDimension; ++i)
            identity_matrix(i, i) = static_cast<Precision>(1);
        return identity_matrix;
    }

    // https://en.wikipedia.org/wiki/Translation_(geometry)
    [[nodiscard]] static constexpr auto
    translation(Vector<Precision, OuterDimension - 1> const& translation_vector) {
        static_assert(OuterDimension == InnerDimension, "Only for square matrices.");
        Matrix<Precision, OuterDimension, OuterDimension> translation_matrix = identity();
        for (auto row{0U}; row < OuterDimension - 1; ++row)
            translation_matrix(row, OuterDimension - 1) = translation_vector[row];
        return translation_matrix;
    }

    // https://en.wikipedia.org/wiki/Scaling_(geometry)
    [[nodiscard]] static constexpr auto
    scaling(Vector<Precision, OuterDimension - 1> const& scaling_vector) {
        static_assert(OuterDimension == InnerDimension, "Only for square matrices.");
        Matrix<Precision, OuterDimension, OuterDimension> scaling_matrix = identity();
        for (auto row{0U}; row < OuterDimension - 1; ++row)
            scaling_matrix(row, row) = scaling_vector[row];
        return scaling_matrix;
    }

    // https://en.wikipedia.org/wiki/Rotation_matrix
    // Rotation around X, Y, or Z axis.
    [[nodiscard]] static constexpr auto rotation(Axis const& axis, Precision const& radians) {
        static_assert(OuterDimension == InnerDimension, "Only for square matrices.");
        static_assert(OuterDimension == 4, "Only for 3D (for now).");
        Matrix<Precision, OuterDimension, OuterDimension> rotation_matrix = identity();
        switch (axis) {
        case Axis::X:
            rotation_matrix(1, 1) = std::cos(radians);
            rotation_matrix(1, 2) = -std::sin(radians);
            rotation_matrix(2, 1) = std::sin(radians);
            rotation_matrix(2, 2) = std::cos(radians);
            return rotation_matrix;
        case Axis::Y:
            rotation_matrix(0, 0) = std::cos(radians);
            rotation_matrix(0, 2) = std::sin(radians);
            rotation_matrix(2, 0) = -std::sin(radians);
            rotation_matrix(2, 2) = std::cos(radians);
            return rotation_matrix;
        case Axis::Z:
            rotation_matrix(0, 0) = std::cos(radians);
            rotation_matrix(0, 1) = -std::sin(radians);
            rotation_matrix(1, 0) = std::sin(radians);
            rotation_matrix(1, 1) = std::cos(radians);
            return rotation_matrix;
        }
        unreachable("Axis has only X,Y,Z");
    }

    [[nodiscard]] static constexpr auto shearing(ShearDirection const& direction) {
        static_assert(OuterDimension == InnerDimension, "Only for square matrices.");
        static_assert(OuterDimension == 4, "Only for 3D (for now).");
        Matrix<Precision, OuterDimension, OuterDimension> shearing_matrix = identity();
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

// Deduct a few commonly used matrices. I don't know if it's possible to use variadic pack to make a
// single guide, or at least deduct that T should be std::common_type of all Ts in the pack.

// 2x2 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0U, Precision const (&)[InnerDimension]>,
       detail::enumerate<1U, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

// 3x3 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0U, Precision const (&)[InnerDimension]>,
       detail::enumerate<1U, Precision const (&)[InnerDimension]>,
       detail::enumerate<2U, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

// 4x4 Matrix
template <typename Precision, std::size_t InnerDimension>
Matrix(detail::enumerate<0U, Precision const (&)[InnerDimension]>,
       detail::enumerate<1U, Precision const (&)[InnerDimension]>,
       detail::enumerate<2U, Precision const (&)[InnerDimension]>,
       detail::enumerate<3U, Precision const (&)[InnerDimension]>)
    -> Matrix<Precision, InnerDimension, InnerDimension>;

using Mat2f = Matrix<float, 2, 2>;  // NOLINT(readability-identifier-naming)
using Mat3f = Matrix<float, 3, 3>;  // NOLINT(readability-identifier-naming)
using Mat4f = Matrix<float, 4, 4>;  // NOLINT(readability-identifier-naming)
using Mat2d = Matrix<double, 2, 2>; // NOLINT(readability-identifier-naming)
using Mat3d = Matrix<double, 3, 3>; // NOLINT(readability-identifier-naming)
using Mat4d = Matrix<double, 4, 4>; // NOLINT(readability-identifier-naming)

extern template class Matrix<double, 4, 4>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_MATRIX_HH_
